#library imports

import pandas as pd
import numpy as np
from pathlib import Path
import logging
import sys

import tensorflow as tf
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler, LabelEncoder
from sklearn.metrics import confusion_matrix, classification_report
import seaborn as sns
import matplotlib.pyplot as plt

path = Path.cwd()
logger = logging.getLogger(__name__)

TRAIN_RESULTS_DIR = Path(path / "train_results")
TRAIN_RESULTS_DIR.mkdir(exist_ok=True)

#data loading
try : 
    data = pd.read_csv(path / "data" / "genre_v2.csv", low_memory=False)
except : 
    logging.critical("Something went wrong with the training data loading",exc_info = True)
    sys.exit()

#data cleaning
try : 
    x = data[['danceability', 'energy', 'key', 'loudness', 'mode', 'speechiness',
              'acousticness', 'instrumentalness', 'liveness', 'valence', 'tempo', 'duration_ms']]
    y = data['genre']
    logging.info("Data loaded")
except : 
    logging.critical("Something went wrong with the data cleaning",exc_info = True)
    exit()
    
try : 
    #transform music styles into int
    encoder = LabelEncoder()
    y = encoder.fit_transform(y)
    
    #separate dat in two data sets
    x_train, x_test, y_train, y_test = train_test_split(x, y, test_size=0.2, random_state=42, stratify=y)

    #Normalize data (more accuracy expected that way)
    scaler = StandardScaler()
    x_train = scaler.fit_transform(x_train)
    x_test = scaler.transform(x_test)
except : 
    logging.critical("impossible to format data in the correct way",exc_info = True)
    exit()


#model construction
try : 
    model = tf.keras.models.Sequential([
        tf.keras.layers.Dense(512, activation='relu', input_shape=(x_train.shape[1],)),
        tf.keras.layers.BatchNormalization(),
        tf.keras.layers.Dropout(0.3),
        
        tf.keras.layers.Dense(256, activation='relu'),
        tf.keras.layers.BatchNormalization(),
        tf.keras.layers.Dropout(0.3),
        
        tf.keras.layers.Dense(128, activation='relu'),
        tf.keras.layers.BatchNormalization(),
        tf.keras.layers.Dropout(0.3),
        tf.keras.layers.Dense(len(encoder.classes_), activation='softmax')  # Activation softmax pour la classification
    ])
    
#model compilation
    model.compile(
        optimizer=tf.keras.optimizers.Adam(learning_rate=0.001),  # Taux d’apprentissage ajustable
        loss=tf.keras.losses.SparseCategoricalCrossentropy(from_logits=False),  # Pour des classes encodées en entiers
        metrics=['accuracy']
    )  
except : 
    logging.critical("creation of model failed",exc_info = True)
    exit()

try : 
    # Définition des callbacks
    early_stopping = tf.keras.callbacks.EarlyStopping(monitor='val_loss', patience=5, restore_best_weights=True)
    reduce_lr = tf.keras.callbacks.ReduceLROnPlateau(monitor='val_loss', factor=0.5, patience=3, min_lr=1e-6)
except : 
    logging.error("definition of callbacks failed",exc_info = True)
    exit()

try : 
    #model training
    history = model.fit(
        x_train, y_train,
        validation_split=0.2,  # 20% des données d'entraînement utilisées pour la validation
        epochs=5,            # Nombre maximum d’époques
        batch_size=32,        # Taille des lots
        callbacks=[early_stopping, reduce_lr],
        verbose=1
    )
    logging.info("Training of the model just ended withut any error")
except : 
    logging.critical("training of model failed", exc_info = True)

try : 
    #evaluate model
    loss, accuracy = model.evaluate(x_test, y_test, verbose=0)
    logging.info(f"Accuracy sur le jeu de test : {accuracy:.2f}")
    
    #Predict training set
    predictions = model.predict(x_test)
    predicted_classes = np.argmax(predictions, axis=1)
    
    #Show results in a matrix
    conf_matrix = confusion_matrix(y_test, predicted_classes)
    plt.figure(figsize=(12, 10))
    sns.heatmap(conf_matrix, annot=True, fmt='d', cmap='Blues', xticklabels=encoder.classes_, yticklabels=encoder.classes_)
    plt.xlabel('Prédictions')
    plt.ylabel('Vérités')
    plt.title('Matrice de confusion')
    plt.savefig(TRAIN_RESULTS_DIR / 'confusion_matrix.pdf')
    
#Report
    logging.info(classification_report(y_test, predicted_classes, target_names=encoder.classes_))

except : 
    logging.info("something went wrong with evaluation of the model, the model has still been trained correctly", exc_info = True)

try : 
    model.save(TRAIN_RESULTS_DIR /'music_genre_classifier.keras')
    logging.info("The model has been saved SUCCESSFULLY")
except : 
    logging.critical("the model has trained correctly but the save has failed",exc_info = True)
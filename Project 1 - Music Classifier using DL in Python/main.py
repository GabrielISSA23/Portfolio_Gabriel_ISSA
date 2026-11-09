import logging
import sys
from pathlib import Path

path = Path.cwd()

#configure directories 
LOG_DIR = Path(path / "logs")
LOG_FILE = LOG_DIR / "application.log"

LOG_DIR.mkdir(exist_ok=True)

try :
    logging.basicConfig(
        level=logging.DEBUG,
        format="%(asctime)s | %(levelname)s | %(name)s | %(message)s",
        handlers=[
            logging.handlers.RotatingFileHandler(
                LOG_FILE,
                maxBytes=1_000_000,  # 1 Mo
                backupCount=5,
                encoding="utf-8"
            ),
            logging.StreamHandler()
        ], 
    )
    
    logger = logging.getLogger(__name__)
    logging.debug("main.py launched, logging configured successfully")
except Exception as e:
    print(f"Something went wrong with the log configuration: {e}")

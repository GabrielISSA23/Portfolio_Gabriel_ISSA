
/*
Context:
This logic is used to identify member activity during the validity period of their one-year subscription to the NoumeaStreaming platform. 
The goal is to see if people are subscribed to Noumea STreaming when they buy 
Logic:
Step 1: Retrieve and parse all NoumeaStreaming subscriptions.
Step 2: Merge subscriptions that overlap or fall within the same date range.
Step 3: Retrieve all member activities that meet both of the following conditions:
Condition 1: The member has subscribed to NoumeaStreaming.
Condition 2: The activity date falls between the start date and end date of one of the member's subscriptions.
If both conditions are met, the activity is flagged as has_sub_noumeastreaming
*/

-- list every subsription mail received by any member 
WITH streaming_sub_tracking AS (
SELECT
    id_member,
    member_name, 
    activity_datetime AS sub_start_date,
    activity_datetime + INTERVAL '1 year' AS sub_end_date,
    ROW_NUMBER() OVER (PARTITION BY id_member ORDER BY activity_datetime) AS sub_rank
FROM
   (streaming_mail.activities UNION ALL client_mail2 ) AS client_mail_all
WHERE
    source_name IN ("noumeastreaming-mailing-1", "noumeastreaming-mailing-2","noumeastreaming-mailing-2")
    AND mail_type = "Subscription" 
    AND member_name NOT IN ["Fabrice","François","Frédérique"] --outliers than needs to be deleted 
),

-- assigns a `period_group_id` to the subscriptions. If two subscriptions fall within the same date range, they will belong to the same group.
grouped_streaming_sub_tracking AS (
SELECT
    a.id_member,
    a.member_name, 
    a.sub_start_date,
    a.sub_end_date,
    -- if sub date start is lower than previous sub end date -> We do not increase the group number and -- if the sub has been ended, we can increment the number of group 
    SUM(CASE WHEN a.sub_start_date <= b.sub_end_date THEN 0 ELSE 1 END )
    -- looks at previous lines of the same member and define the correct number to use 
    OVER (PARTITION BY a.id_member ORDER BY a.sub_start_date ROWS BETWEEN UNBOUNDED PRECEDING AND CURRENT ROW) AS period_group_id  
FROM streaming_sub_tracking a
LEFT JOIN streaming_sub_tracking bON a.id_member = b.id_member AND a.sub_rank = b.sub_rank + 1),

--group subscription that has the same number are merged 
merged_streaming_sub_tracking AS (
SELECT
    id_member,
    MAX(sub_end_date) AS merged_sub_end
    MIN(sub_start_date) AS merged_sub_start,
FROM grouped_streaming_sub_tracking
GROUP BY id_member, period_group_id
)


SELECT
    act.fk_member AS id_member,
    act.member_name, 
    activity_uid,
    DATE(merged_sub_start) AS start_sub_noumeastreaming,
    DATE(merged_sub_end) AS end_sub_noumeastreaming,
    CASE WHEN activity_datetime BETWEEN merged_sub_start AND merged_sub_end THEN 1 ELSE 0 END AS has_sub_noumeastreaming
FROM
    streaming.activities act
    LEFT JOIN merged_streaming_sub_tracking ms ON act.fk_member = ms.id_member 
WHERE 
    fk_member IS NOT NULL
    AND activity_datetime > "2000-01-01"
    AND market_code == "France"
    AND activity_number > 0

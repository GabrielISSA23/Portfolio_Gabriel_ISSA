
/*
Context:
This logic is used to identify member activity during the validity period of their one-year subscription to the NoumeaStreaming platform. 
The goal is to see if people are subscribed to NoumeaStreaming when they buy a subscription to an other streaming platform
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
   (streaming.client_mails1 UNION ALL streaming.client_mails2) AS client_mail_all
WHERE
    source_name IN ("noumeastreaming-mailing", "France-noumeastreaming-mailing")
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

import psycopg2
from db_config import DB_PASSWORD
conn = psycopg2.connect(
    host="localhost",
    dbname="daily_checklist",
    user="postgres",
    password=DB_PASSWORD
)
print("connected")
conn.close()
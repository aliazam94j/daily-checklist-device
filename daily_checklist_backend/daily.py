from flask import Flask, request
from datetime import datetime
import psycopg2
from db_config import DB_PASSWORD


app = Flask(__name__)

def get_conn():
    return psycopg2.connect(
        host="localhost",
        dbname="daily_checklist",
        user="postgres",
        password=DB_PASSWORD
    )

@app.route("/DAILY", methods=["POST"])
def daily_post():
    now = datetime.now()
    conn = get_conn()
    cur = conn.cursor()
    cur.execute("INSERT INTO press (pressed_at) VALUES (%s)", (str(now),))
    conn.commit()
    cur.close()
    conn.close()
    print("Button pressed successfully!")
    return "ok", 200

@app.route("/")
def home():
    conn = get_conn()
    cur = conn.cursor()
    cur.execute("SELECT pressed_at FROM press ORDER BY id DESC LIMIT 1")
    row = cur.fetchone()
    cur.close()
    conn.close()
    if row is None:
        return "Last press: None"
    return f"Last press: {row[0]}"

def init_db():
    conn = get_conn()
    cur = conn.cursor()
    cur.execute("""
        CREATE TABLE IF NOT EXISTS press (
            id SERIAL PRIMARY KEY,
            pressed_at TEXT
        )
    """)
    conn.commit()
    cur.close()
    conn.close()

if __name__ == "__main__":
    init_db()
    app.run(host="0.0.0.0", port=5000)


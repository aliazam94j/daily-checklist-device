from flask import Flask, request
from datetime import datetime
import sqlite3


# http://10.60.84.41:5000
app = Flask(__name__)

@app.route("/DAILY",methods=["POST"])
def daily_post():
    now = datetime.now()
    conn = sqlite3.connect("presses.db")
    conn.execute("INSERT INTO press(pressed_at) VALUES (?)",
                 (str(now),))
    conn.commit()
    conn.close()
    print("Button pressed successfully!")
    return "ok", 200

@app.route("/")
def home():
    conn = sqlite3.connect("presses.db")
    row = conn.execute("SELECT pressed_at FROM press ORDER BY id DESC LIMIT 1").fetchone()
    conn.close()
    if row is None:
        return "Last press: None"
    return f"Last press: {row[0]}"

def init_db():
    conn = sqlite3.connect("presses.db") # open
    conn.execute("""
    CREATE TABLE IF NOT EXISTS press(
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    pressed_at TEXT
    ) 
    """
)# do the work
    conn.commit() # save the changes
    conn.close() # close


if __name__ == "__main__":
    init_db()
    app.run(host="0.0.0.0", port=5000)

import random
from sqlite3 import Error
import sqlite3
import time


def insertRandomData(conn):
    cursor = conn.cursor()

    # Generate and insert random data
    for i in range(10):  # Change 10 to the number of data points you want to insert
        timestamp = int(time.time() + round(random.uniform(1, 300)))
        temperature = round(
            random.uniform(20, 30), 2
        )  # Random temperature between 20 and 30 degrees Celsius
        humidity = round(
            random.uniform(40, 60), 2
        )  # Random humidity between 40% and 60%
        pressure = round(
            random.uniform(900, 1100), 2
        )  # Random pressure between 900 and 1100 hPa
        air_quality = round(
            random.uniform(0, 100), 2
        )  # Random air quality between 0 and 100
        laser_distance = round(
            random.uniform(0, 100), 2
        )  # Random distance from laser sensor between 0 and 100 cm
        ultrasonic_distance = round(
            random.uniform(0, 200), 2
        )  # Random distance from ultrasonic sensor between 0 and 200 cm
        lightness = round(
            random.uniform(0, 100), 2
        )  # Random lightness between 0 and 100

        # Insert data into the table
        cursor.execute(
            """INSERT INTO sensor_data 
                        (timestamp, temperature, humidity, pressure, air_quality, laser_distance, ultrasonic_distance, lightness) 
                        VALUES (?, ?, ?, ?, ?, ?, ?, ?)""",
            (
                timestamp,
                temperature,
                humidity,
                pressure,
                air_quality,
                laser_distance,
                ultrasonic_distance,
                lightness,
            ),
        )

    # Commit changes and close connection
    conn.commit()
    conn.close()


def create_connection(db_file):
    """create a database connection to a SQLite database"""
    connection = None
    try:
        connection = sqlite3.connect(db_file)
        cursor = connection.cursor()

        # Create a table to store sensor data
        cursor.execute(
            """CREATE TABLE IF NOT EXISTS sensor_data
                        (id INTEGER PRIMARY KEY,
                        timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
                        temperatures_id REAL,
                        humidity REAL,
                        pressure REAL,
                        air_quality REAL,
                        laser_distance REAL,
                        ultrasonic_distance REAL,
                        lightness REAL);
                CREATE TABLE IF NOT EXISTS temperatures 
                        (id INTEGER PRIMARY KEY,
                        terometer_id INTEGER,
                        temperature REAL);
                """
        )
        connection.commit()
        insertRandomData(connection)
        connection.close()

    except Error as e:
        print(e)
    finally:
        if connection:
            connection.close()


if __name__ == "__main__":
    create_connection(r"./database.db")

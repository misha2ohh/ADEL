import pandas as pd

# Step 1: Load file
df = pd.read_csv("jn_data_v1.csv", dtype=str)

# Step 2: Strip whitespace
df = df.applymap(lambda x: x.strip() if isinstance(x, str) else x)

# Step 3: Convert key columns to numeric
numeric_cols = [
    "timestamp_ms", "button_pressed", "correct_press", "missed_opportunity", 
    "vibration_event", "vibration_duration_ms", "vibration_count", "score", "time_left_ms"
]
for col in numeric_cols:
    df[col] = pd.to_numeric(df[col], errors='coerce')

# Step 4: Generate round_id from score reset
df["round_id"] = (df["score"].diff() < 0).cumsum()

# Detect new rounds based on time gaps larger than 10 seconds
df["timestamp_ms"] = pd.to_numeric(df["timestamp_ms"], errors="coerce")
df["time_diff"] = df["timestamp_ms"].diff().fillna(0)
df["round_id"] = (df["time_diff"] > 10000).cumsum()  # 10 sec = 10000 ms


# Step 5: Clean up progress field
if "progress" in df.columns:
    df["progress"] = df["progress"].fillna("")

# Step 6: Save cleaned file
df.to_csv("jn_data_cleaned.csv", index=False)
print("✅ Cleaned file saved as 'jn_data_cleaned.csv'")

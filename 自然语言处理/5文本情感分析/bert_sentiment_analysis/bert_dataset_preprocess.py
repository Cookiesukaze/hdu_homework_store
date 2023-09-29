import pandas as pd


def calculate_rating(score):
    if score == 50:
        return 5
    elif score == 40:
        return 4
    elif score == 30:
        return 3
    elif score == 20:
        return 2
    else:
        return 1


df = pd.read_csv("流浪地球_preprocess.csv",encoding="ANSI")

df['scoresForBert'] = df['scores'].apply(calculate_rating)

# 写入新的Rating列到csv
df.to_csv('流浪地球_preprocess_bert.csv', index=False,encoding="ANSI",header=False)

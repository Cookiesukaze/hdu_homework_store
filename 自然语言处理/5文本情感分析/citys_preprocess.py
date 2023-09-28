import cpca
import pandas as pd
import re

# cn_division = [
#     '安徽省', '北京市', '重庆市', '福建省', '甘肃省', '广东省', '广西壮族自治区',
#     '贵州省', '海南省', '河北省', '河南省', '黑龙江省', '湖北省', '湖南省', '吉林省',
#     '江苏省', '江西省', '辽宁省', '内蒙古自治区', '宁夏回族自治区', '青海省',
#     '山东省', '山西省', '陕西省', '上海市', '四川省', '天津市',
#     '西藏自治区', '新疆维吾尔自治区', '云南省', '浙江省',
#     '台湾省',
#     '香港特别行政区', '澳门特别行政区'
# ]


def citys_preprocess_func(df_copy):
    df_citys = df_copy['citys'].values.tolist()
    temp = []
    for city in df_citys:
        city = city.strip("'[]")
        if city == '':
            city = "未知"
        temp.append(city)
    df_citys = temp
    # print(df_citys)
    df_cn_citys = cpca.transform(df_citys)["省"].values.tolist()
    temp = []
    for index, city in enumerate(df_citys):
        if df_cn_citys[index] is not None:
            city = df_cn_citys[index]
        elif df_citys[index] != '未知':
            city = "国外"
        else:
            city = df_citys[index]
        temp.append(city)
    df_citys = temp
    return df_citys

# 测试
# df = pd.read_csv("流浪地球.csv", encoding='ANSI')
# citys_preprocess_func(df)

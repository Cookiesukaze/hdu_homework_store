


def dict_creator(ori_list):
    dict = {}
    for piece in ori_list:
        if piece not in dict:
            dict[piece] = 0
        dict[piece] += 1
    return dict
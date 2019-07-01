from json_converse import get_json_from_sen,CTLData_to_json,cmp_LF
from Serial_Module import write_ser
def data_trasmit(str_rec):
    
    if str_rec == None:
        data1 = [0.0,0.0,0]
        data2 = [0.0,0.0,0]
        data3 = [0.0,0.0]
        data4 = [0.0,0.0]
        data5 = [0.0,0.0]
        data6 = [0.0,0.0]
        return data1,data2,data3,data4,data5,data6
    
    data1 = [0.0,0.0,0]
    data2 = [0.0,0.0,0]
    data3 = [0.0,0.0]
    data4 = [0.0,0.0]
    data5 = [0.0,0.0]
    data6 = [0.0,0.0]
    
    #str_rec = "@1,12.95,12,13#@2,21.8,22,23#@3,38.1,32#@4,41,42#@5,51,52#@6,61,62#"
    s1 = str_rec.split('@')
    s1.pop(0)
    for i in range(0, len(s1)):
        s1[i] = s1[i].replace('#', '')
    sen_data = []
    for i in range(0,len(s1)):
        tmp = s1[i].split(',')
        sen_data.append(tmp)
        if len(tmp) > 2:
            for i in range(0,len(sen_data)):
                tmp = sen_data[i]


                if tmp[0] == "1":
                    tmp.pop(0)
                    tmp[0] = float(tmp[0])
                    tmp[1] = float(tmp[1])
                    tmp[2] = int(tmp[2])
                    data1 = tmp
                if tmp[0] == "2":
                    tmp.pop(0)
                    tmp[0] = float(tmp[0])
                    tmp[1] = float(tmp[1])
                    tmp[2] = int(tmp[2])
                    data2 = tmp
                if tmp[0] == "3":
                    tmp.pop(0)
                    tmp[0] = float(tmp[0])
                    tmp[1] = float(tmp[1])
                    data3 = tmp
                if tmp[0] == "4":
                    tmp.pop(0)
                    tmp[0] = float(tmp[0])
                    tmp[1] = float(tmp[1])
                    data4 = tmp
                if tmp[0] == "5":
                    tmp.pop(0)
                    tmp[0] = float(tmp[0])
                    tmp[1] = float(tmp[1])
                    data5 = tmp
                if tmp[0] == "10":
                    tmp.pop(0)
                    tmp[0] = float(tmp[0])
                    tmp[1] = float(tmp[1])
                    data6 = tmp
        # print(data1)
        # print(data2)
        # print(data3)
        # print(data4)
        # print(data5)
        # print(data6)
        #print(sen_data)
            get_json_from_sen(data1,data2,data3,data4,data5,data6)

        if len(tmp) == 2:
            if tmp[1] == 0:
                ctl1,ctl2,ctl3,ctl4,ctl5 = cmp_LF()
                ctl_arr = [ctl1,ctl2,ctl3,ctl4,ctl5]
                ctl_arr[(tmp[0]-1)] = 0
                CTLData_to_json(ctl1,ctl2,ctl3,ctl4,ctl5)
                write_ser("@%d,0"%tmp[0])
            tmp = []

    return data1,data2,data3,data4,data5,data6

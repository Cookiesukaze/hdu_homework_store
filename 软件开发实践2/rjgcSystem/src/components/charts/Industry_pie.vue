<template>
    <div>
        <div id="Industry_pie" :style="{ width: '600px', height: '250px' }"></div>
    </div>
</template>

<script>
import axios from "axios";
//接口完成
export default {
    name: 'Industry_pie',
    data() {
        return {
            violateNum:[
                {
                    name: '酒店业',
                    value: 70
                },
                {
                    name: '零售业',
                    value: 68
                },
                {
                    name: '制造业',
                    value: 48
                },
                {
                    name: 'IT服务',
                    value: 40
                },
                {
                    name: '建筑业',
                    value: 32
                },
                {
                    name: '金融业',
                    value: 27
                },
            ],
            respawnNum:[
                {
                    name: '酒店业',
                    value: 70
                },
                {
                    name: '零售业',
                    value: 68
                },
                {
                    name: '制造业',
                    value: 48
                },
                {
                    name: 'IT服务',
                    value: 40
                },
                {
                    name: '建筑业',
                    value: 32
                },
                {
                    name: '金融业',
                    value: 27
                },
            ],
        }
    },
    mounted() {
        axios.get("http://localhost:8081/respawn/industry",
            ).then(res=> {
            console.log(res.data)
            for(var i1=0;i1<6;i1++) {this.violateNum[i1].value=res.data.data[1][i1]}
            for(var i2=0;i2<6;i2++) {this.respawnNum[i2].value=res.data.data[4][i2]}
            this.drawIndustry_pie()
        })
    },
    methods: {
        drawIndustry_pie() {
            let Industry_pie = this.$echarts.init(document.getElementById('Industry_pie'))
            let option = {
                tooltip: {
                    formatter: function (params, ticket, callback) {
                        // return params.seriesName + '<br />' + params.name + '：' + params.value
                        return params.name + '：' + params.value +"个"
                    }//数据格式化
                },
                // color:['#516b91','#54d6b6','#a6db69','#ffd454','#ffa361','#d1d1d1'],
                color:['#516b91','#59c4e6','#93b7e3','#edafda','#a5e7f0','#cbb0e3'],
                title: [
                    {
                        left: 'center'
                    },
                    {
                        subtext: '违约占比',
                        left: 150,
                        top: 210,
                        textAlign: 'center'
                    },
                    {
                        subtext: '重生占比',
                        left: 450,
                        top: 210,
                        textAlign: 'center'
                    },
                ],
                series: [
                    {
                        type: 'pie',
                        minAngle: 15,//最小角度
                        startAngle:270, //起始角度
                        radius: '50%',
                        center: ['50%', '50%'],
                        data: this.violateNum,
                        label: {
                            position: 'outer',
                            alignTo: 'none',
                            bleedMargin: 5
                        },
                        left: -300,
                        right: 0,
                        top: 0,
                        bottom: 0
                    },
                    {
                        type: 'pie',
                        radius: '50%',
                        center: ['50%', '50%'],
                        data: this.respawnNum,
                        label: {
                            position: 'outer',
                            alignTo: 'labelLine',
                            bleedMargin: 5
                        },
                        left: 300,
                        right: 0,
                        top: 0,
                        bottom: 0
                    },
                ]
            };
            Industry_pie.setOption(option)
        },
    },
}
</script>

<style>
</style>
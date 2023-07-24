<template>
    <div>
        <div id="Industry_bar" :style="{ width: '600px', height: '400px' }"></div>
    </div>
</template>

<script>
//接口完成
import axios from "axios";

export default {
    name: 'Industry_bar',
    data() {
        return {
            violateNum:[
                2.0, 4.9, 7.0, 23.2, 25.6, 76.7, 135.6, 162.2, 32.6, 20.0, 6.4, 3.3
            ],
            respawnNum:[
                2.6, 5.9, 9.0, 26.4, 28.7, 70.7, 175.6, 182.2, 48.7, 18.8, 6.0, 2.3
            ],
            violateRate:[
                2.0, 2.2, 3.3, 4.5, 6.3, 10.2, 20.3, 23.4, 23.0, 16.5, 12.0, 6.2
            ],
            respawnRate:[
                1.0, 3.2, 2.3, 6.5, 4.3, 11.2, 15.3, 22.4, 26.0, 11.5, 13.0, 8.2
            ],
        }
    },
    mounted() {
        axios.get("http://localhost:8081/respawn/industry",
            {params:{
                    id:111948682
                }
            }).then(res=> {
            console.log(res.data)
            this.violateNum=res.data.data[0]
            this.respawnNum=res.data.data[3]
            this.violateRate=res.data.data[2];
            this.respawnRate=res.data.data[5];
            this.drawIndustry_bar()
        })
    },
    methods: {
        drawIndustry_bar() {
            let Industry_bar = this.$echarts.init(document.getElementById('Industry_bar'))
            let option = {
                color:['#516b91','#59c4e6','#93b7e3','#edafda','#a5e7f0','#cbb0e3'],
                tooltip: {
                    trigger: 'axis',
                    axisPointer: {
                        type: 'cross',
                        crossStyle: {
                            color: '#999'
                        }
                    }
                },
                toolbox: {
                    feature: {
                        // dataView: { show: true, readOnly: false },
                        // magicType: { show: true, type: ['line', 'bar'] },
                        // restore: { show: true },
                        // saveAsImage: { show: true }
                    }
                },
                legend: {
                    data: ['违约数', '重生数', '违约增长率', '重生增长率']
                },
                xAxis: [
                    {
                        type: 'category',
                        data: ['酒店业', '零售业', '制造业', 'IT服务', '建筑业', '金融业'],
                        axisPointer: {
                            type: 'shadow'
                        }
                    }
                ],
                yAxis: [
                    {
                        type: 'value',
                        name: '主体个数',
                        min: 0,
                        max: 10,
                        interval: 50,
                        axisLabel: {
                            formatter: '{value} 个'
                        }
                    },
                    {
                        type: 'value',
                        name: '增长率',
                        min: 0,
                        max: 30,
                        interval: 5,
                        axisLabel: {
                            formatter: '{value} %'
                        }
                    }
                ],
                series: [
                    {
                        name: '违约数',
                        type: 'bar',
                        tooltip: {
                            valueFormatter: function (value) {
                                return value + ' 个';
                            }
                        },
                        data: this.violateNum
                    },
                    {
                        name: '重生数',
                        type: 'bar',
                        tooltip: {
                            valueFormatter: function (value) {
                                return value + ' 个';
                            }
                        },
                        data: this.respawnNum
                    },
                    {
                        name: '违约增长率',
                        type: 'line',
                        yAxisIndex: 1,
                        tooltip: {
                            valueFormatter: function (value) {
                                return value + ' %';
                            }
                        },
                        data: this.violateRate,
                    },
                    {
                        name: '重生增长率',
                        type: 'line',
                        yAxisIndex: 1,
                        tooltip: {
                            valueFormatter: function (value) {
                                return value + ' %';
                            }
                        },
                        data: this.respawnRate,
                    }
                ]
            };
            Industry_bar.setOption(option)
        },
    },
}
</script>

<style>
</style>
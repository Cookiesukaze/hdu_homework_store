<template>
    <div>
        <div id="China_map" :style="{width:'750px',height:'600px'}"></div>

    </div>
</template>
<script>
//接口完成
import echarts from 'echarts/dist/echarts.js'
import 'echarts/map/js/china.js'
import chinaJson from './china.json'
import axios from "axios"; // 这个是json引用
var violateData1 = [
    {name: '东北', value: ""},
    {name: '华北', value: 1},
    {name: '西北', value: 2},
    {name: '西南', value: 9},
    {name: '华中', value: 3},
    {name: '华南', value: 1},
    {name: '华东', value: 2},
    {name: '港澳台', value: 5},
];
var violateData2= [
    {name: '东北', value: 0.8},
    {name: '华北', value: 0.7},
    {name: '西北', value: 0.2},
    {name: '西南', value: 0.9},
    {name: '华中', value: 0.1},
    {name: '华南', value: 0.2},
    {name: '华东', value: 0.5},
    {name: '港澳台', value: 0.8},
];
var violateData3=[
    {name: '东北', value: 0.2},
    {name: '华北', value: 0.4},
    {name: '西北', value: 0.5},
    {name: '西南', value: 0.8},
    {name: '华中', value: 0.9},
    {name: '华南', value: 0.6},
    {name: '华东', value: 0.5},
    {name: '港澳台', value: 0.1},
];
var respawnData1 =[
    {name: '东北', value: 3},
    {name: '华北', value: 7},
    {name: '西北', value: 9},
    {name: '西南', value: 2},
    {name: '华中', value: 1},
    {name: '华南', value: 8},
    {name: '华东', value: 6},
    {name: '港澳台', value: 6},
];
var respawnData2= [
    {name: '东北', value: 0.1},
    {name: '华北', value: 0.5},
    {name: '西北', value: 0.2},
    {name: '西南', value: 0.9},
    {name: '华中', value: 0.4},
    {name: '华南', value: 0.8},
    {name: '华东', value: 0.2},
    {name: '港澳台', value: 0.2},
];
var respawnData3=[
    {name: '东北', value: 0.9},
    {name: '华北', value: 0.7},
    {name: '西北', value: 0.1},
    {name: '西南', value: 0.1},
    {name: '华中', value: 0.2},
    {name: '华南', value: 0.8},
    {name: '华东', value: 0.3},
    {name: '港澳台', value: 0.3},
];
var select1Max=10;
var select2Max=2;
var select3Max=2;
var select4Max=10;
var select5Max=2;
var select6Max=2;
var select1Min=4;
var select2Min=0;
var select3Min=0;
var select4Min=4;
var select5Min=0;
var select6Min=0;
export default {
    name: 'China_map',
    props: ["selectItem"],
        data() {
            return {


                selectData: violateData1,
                chinaDaquGeo: {},
                selectMax: select1Max,
                selectMin: select1Min,
            }
        },
        watch: {
            selectItem: function (val) {
                console.log("中国地图组件获得了数据选项：" + this.selectItem);
                if (val == '1') {
                    this.selectData = violateData1;
                    this.selectMax = select1Max;
                    this.selectMin = select1Min;
                }
                else if (val == '2') {
                    this.selectData = violateData2;
                    this.selectMax = select2Max;
                    this.selectMin = select2Min;
                }
                else if (val == '3') {
                    this.selectData = violateData3;
                    this.selectMax = select3Max;
                    this.selectMin = select3Min;
                }
                else if (val == '4') {
                    this.selectData = respawnData1;
                    this.selectMax = select4Max;
                    this.selectMin = select4Min;
                }
                else if (val == '5') {
                    this.selectData = respawnData2;
                    this.selectMax = select5Max;
                    this.selectMin = select5Min;
                }
                else {
                    this.selectData = respawnData3;
                    this.selectMax = select6Max;
                    this.selectMin = select6Min;
                }
                console.log("中国地图组件获得了数据：" + this.selectData);
                console.log("中国地图组件获得了数据最大值：" + this.selectMax);
                this.drawChina_map()
            },

        },
        methods: {
            randomValue() {
                return Math.round(Math.random() * 1000);
            },
            drawChina_map() {
                let China_map = this.$echarts.init(document.getElementById('China_map'));
                let option = {

                    tooltip: {
                        formatter: function (params, ticket, callback) {
                            // return params.seriesName + '<br />' + params.name + '：' + params.value
                            return params.name + '：' + params.value
                        }//数据格式化
                    },
                    visualMap: {
                        min: this.selectMin,
                        max: this.selectMax,
                        left: 'left',
                        top: 'bottom',
                        type: "piecewise",
                        //text: ['高','低'],//取值范围的文字
                        inRange: {
                            color: ['#9fc1e1', '#516b91']//取值范围的颜色
                        },
                        show: true//图注
                    },
                    geo: {
                        map: 'china',
                        roam: true,//不开启缩放和平移
                        zoom: 1,//视角缩放比例
                        center: [108.5525, 34.3227],
                        label: {
                            normal: {
                                show: true,
                                fontSize: '14',
                                fontFamily:'opposans',
                                color: 'rgb(49,49,49)',
                                textBorderColor: "#ffffff",
                                textBorderWidth:'10px',
                                fontWeight:'bold',
                            }
                        },
                        itemStyle: {
                            normal: {
                                borderColor: 'rgba(0, 0, 0, 0.2)'
                            },
                            emphasis: {
                                areaColor: '#ffffff',//鼠标选择区域颜色
                                shadowOffsetX: 0,
                                shadowOffsetY: 0,
                                shadowBlur: 20,
                                borderWidth: 0,
                                shadowColor: 'rgba(0, 0, 0, 0.5)'
                            }
                        }
                    },
                    series: [
                        {
                            type: 'map',
                            geoIndex: 0,
                            data: this.selectData,
                        },
                    ]
                };
                China_map.setOption(option)
                /*this.mapCharts.on('click', function (params) {
              alert(params.name);
            });*/
            },
            //划分大区
            mergeProvinces(chinaJson, names, properties) {//合并大区里省份的coordinates
                var features = chinaJson.features;
                var polygons = [];
                for (var i = 0; i < names.length; i++) {
                    var polygonsCoordinates = [];
                    for (var z = 0; z < names[i].length; z++) {
                        for (var j = 0; j < features.length; j++) {
                            if (features[j].properties.name == names[i][z]) {
                                if (features[j].geometry.coordinates[0].constructor == String) {//合并coordinates
                                    for (var l = 0; l < features[j].geometry.coordinates.length; l++) {
                                        polygonsCoordinates.push(features[j].geometry.coordinates[l]);
                                    }

                                } else if (features[j].geometry.coordinates[0].constructor == Array) {
                                    for (var k = 0; k < features[j].geometry.coordinates.length; k++) {
                                        for (var d = 0; d < features[j].geometry.coordinates[k].length; d++) {
                                            polygonsCoordinates.push(features[j].geometry.coordinates[k][d]);
                                        }
                                    }
                                }
                                break;
                            }
                        }
                    }
                    polygons.push(polygonsCoordinates);
                }

                // 构建新的合并区域
                var features = [];

                for (var a = 0; a < names.length; a++) {
                    var feature = {
                        id: features.length.toString(),
                        type: "FeatureCollection",
                        geometry: {
                            type: "Polygon",
                            coordinates: polygons[a]
                        },
                        properties: {
                            name: properties.name[a] || "",
                            childNum: polygons[a].length
                        }
                    };
                    if (properties.cp[a]) {
                        feature.properties.cp = properties.cp[a];
                    }

                    // 将新的合并区域添加到地图中
                    features.push(feature);
                }
                this.chinaDaquGeo.type = "FeatureCollection";
                this.chinaDaquGeo.features = features;
            },

            repRegionStrategy() {
                var params = {
                    names: [//把各个大区的省份用二维数组分开
                        ['北京', '天津', '河北', '山西', '内蒙古'],
                        ["黑龙江", "吉林", "辽宁"],
                        ['山东', '江苏', '安徽', '江西', '浙江', '福建', '上海',],
                        ['河南', '湖北', '湖南'],
                        ['广东', '广西', '海南'],
                        ['重庆', '四川', '云南', '西藏', '贵州'],
                        ['陕西', '甘肃', '青海', '宁夏', '新疆'],
                        ['香港', '澳门', '台湾'],
                    ],
                    properties: {//自定义大区的名字，要和上面的大区省份一一对应
                        name: ['华北', '东北', '华东', '华中', '华南', '西南', '西北', '港澳台'],
                        cp: [//经纬度可以自己随意定义
                            [116.24, 39.54],
                            [126.32, 43.50],
                            [121.28, 31.13],
                            [114.20, 30.32],
                            [113.15, 23.08],
                            [104.04, 30.39],
                            [103.49, 36.03],
                            [116.47, 23.15],
                        ]
                    }
                };
                this.mergeProvinces(chinaJson, params.names, params.properties);
            }
        },
        //调用
        created() {
            this.repRegionStrategy()
            // 注册地图
            this.$echarts.registerMap('china', this.chinaDaquGeo) // 如果是js引入就不需要这一行了
        },
        mounted() {
            axios.get("http://localhost:8081/respawn/area",
            ).then(res=> {
                console.log(res.data)
                for(var i1=0;i1<8;i1++) {violateData1[i1].value=res.data.data[0][i1]}
                for(var i2=0;i2<8;i2++) {violateData2[i2].value=res.data.data[1][i2]}
                for(var i3=0;i3<8;i3++) {violateData3[i3].value=res.data.data[2][i3]}
                for(var i4=0;i4<8;i4++) {respawnData1[i4].value=res.data.data[3][i4]}
                for(var i5=0;i5<8;i5++) {respawnData2[i5].value=res.data.data[4][i5]}
                for(var i6=0;i6<8;i6++) {respawnData3[i6].value=res.data.data[5][i6]}
                this.drawChina_map()
            })
            this.$nextTick(function () {
                this.drawChina_map()
            })

        }
    }
</script>
<style>
#China_map{
    height:500px;
}
</style>
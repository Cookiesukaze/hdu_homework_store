<template>
    <div style="position: absolute;min-width: 1330px;margin-left: 10px;overflow-y: hidden;margin-top:-3px;">
        <div style="margin-left: 50px;margin-top:30px;display: flex;flex-direction:row;">
            <a-col style="width: 210px;  margin-right:11px">
                <a-card>
                    <div class="name-desc">酒店业</div>
                    <div>
                        <div style="display: flex;flex-direction: row;margin-top:10px">
                            <div style="display: flex;flex-direction: row;margin-right: 13px">
                                <div class="normal-desc">共</div>
                                <div class="important-desc">{{ totalNum[0] }}</div>
                                <div class="normal-desc">个</div>
                            </div>
                        </div>
                    </div>
                </a-card>
            </a-col>
            <a-col style="width: 210px;  margin-right:11px">
                <a-card>
                    <div class="name-desc">零售业</div>
                    <div>
                        <div style="display: flex;flex-direction: row;margin-top:10px">
                            <div style="display: flex;flex-direction: row;margin-right: 13px">
                                <div class="normal-desc">共</div>
                                <div class="important-desc">{{ totalNum[1] }}</div>
                                <div class="normal-desc">个</div>
                            </div>
                        </div>
                    </div>
                </a-card>
            </a-col>
            <a-col style="width: 210px;  margin-right:11px">
                <a-card>
                    <div class="name-desc">制造业</div>
                    <div>
                        <div style="display: flex;flex-direction: row;margin-top:10px">
                            <div style="display: flex;flex-direction: row;margin-right: 13px">
                                <div class="normal-desc">共</div>
                                <div class="important-desc">{{ totalNum[2] }}</div>
                                <div class="normal-desc">个</div>
                            </div>
                        </div>
                    </div>
                </a-card>
            </a-col>
            <a-col style="width: 210px;  margin-right:11px">
                <a-card>
                    <div class="name-desc">IT服务</div>
                    <div>
                        <div style="display: flex;flex-direction: row;margin-top:10px">
                            <div style="display: flex;flex-direction: row;margin-right: 13px">
                                <div class="normal-desc">共</div>
                                <div class="important-desc">{{ totalNum[3] }}</div>
                                <div class="normal-desc">个</div>
                            </div>
                        </div>
                    </div>
                </a-card>
            </a-col>
            <a-col style="width: 210px;  margin-right:11px">
                <a-card>
                    <div class="name-desc">建筑业</div>
                    <div>
                        <div style="display: flex;flex-direction: row;margin-top:10px">
                            <div style="display: flex;flex-direction: row;margin-right: 13px">
                                <div class="normal-desc">共</div>
                                <div class="important-desc">{{ totalNum[4] }}</div>
                                <div class="normal-desc">个</div>
                            </div>
                        </div>
                    </div>
                </a-card>
            </a-col>
            <a-col style="width: 210px;  margin-right:11px">
                <a-card>
                    <div class="name-desc">金融业</div>
                    <div>
                        <div style="display: flex;flex-direction: row;margin-top:10px">
                            <div style="display: flex;flex-direction: row;margin-right: 13px">
                                <div class="normal-desc">共</div>
                                <div class="important-desc">{{ totalNum[5] }}</div>
                                <div class="normal-desc">个</div>
                            </div>
                        </div>
                    </div>
                </a-card>
            </a-col>
        </div>
        <div style="margin-left: 50px;display: flex;flex-direction: row">
            <div style="display: flex;flex-direction:column;">
                <Industry_pie style="margin-left: 50px;"></Industry_pie>
                <Industry_bar style="margin-left: 50px;"></Industry_bar>
            </div>
            <div style="display: flex;flex-direction: column;">
                <div style="display: flex;flex-direction: row;">
                    <a-select v-model="s1" @change="handleChange()" size="large" style="position:absolute;z-index:99999999;width:160px;margin-top:20px;margin-left: 50px">
                        <a-select-option value="violateSelect">违约分布</a-select-option>
                        <a-select-option value="respawnSelect">重生分布</a-select-option>
                    </a-select>
                    <a-select v-model="s2" @change="handleChange()" size="large" style="position:absolute;z-index:99999999;width:160px;margin-top:20px;margin-left: 240px">
                        <a-select-option value="numSelect">主体总数</a-select-option>
                        <a-select-option value="opacitySelect">主体占比</a-select-option>
                        <a-select-option value="rateSelect">增长趋势</a-select-option>
                    </a-select>
                </div>
                <china_map :selectItem="selectItem" style="margin-left: 50px"></china_map>
            </div>

        </div>
    </div>
</template>

<script>
//
import VA_form from "@/components/charts/VA_form.vue";
import Industry_bar from "@/components/charts/Industry_bar.vue";
import Industry_pie from "@/components/charts/Industry_pie.vue";
import China_map from "@/components/charts/China_map.vue";
import axios from "axios";

export default {
    data() {
        return {
            totalNum:[1,2,3,4,5,6],
            s1:"violateSelect",
            s2:"numSelect",
            selectItem:'',//注意，data不能用methods方法返回值
        }
    },
    computed:{

    },
    components: {
        Industry_pie,
        Industry_bar,
        VA_form,
        China_map,
    },
    mounted(){
        axios.get("http://43.139.140.34:8081/wrong",{
            params:{
                id:111948682
            }
        }).then(res=>{
            // console.log(res.data);
            // for(var i=0;i<6;i++)this.totalNum[i]=res.data.data[i].topicName;
            this.load();
        })
    },
    methods: {
        handleChange() {
            console.log("正在计算违约统计选择项："+this.s1+"和"+this.s2);
            console.log("上一个违约统计选择项："+this.selectItem);
            if (this.s1==undefined && this.s2==undefined)this.selectItem= '1';
            else if(this.s1=='violateSelect' && this.s2=='numSelect')this.selectItem= '1';
            else if(this.s1=='violateSelect' && this.s2=='opacitySelect')this.selectItem= '2';
            else if(this.s1=='violateSelect' && this.s2=='rateSelect')this.selectItem= '3';
            else if(this.s1=='respawnSelect' && this.s2=='numSelect')this.selectItem= '4';
            else if(this.s1=='respawnSelect' && this.s2=='opacitySelect')this.selectItem= '5';
            else this.selectItem= '6';
        },
    },
}
</script>

<style>

/*边框轮子1号*/
.column_1 {
    padding: 0px; /*高度*/
    border-radius: 5px; /*圆角*/
    display: flex;
    flex-direction: column;
}

.space {
    margin-top: 10px; /*间距*/
}

.space_7 {
    margin-top: 7px; /*间距*/
}

.space_15 {
    margin-top: 15px; /*间距*/
}

::-webkit-scrollbar {
    width: 0 !important;
}

::-webkit-scrollbar {
    width: 0 !important;
    height: 0;
}

.name-desc {
    color: rgba(140, 140, 140);
    font-size: 15px;
    margin-left: 2px;
}

.normal-desc {
    color: #4F4F4F;
    font-size: 16px;
    margin-left: 2px;
}

.important-desc {
    color: #4F4F4F;
    font-size: 22px;
    margin-top: -7px;
    margin-left: 2px;
}
</style>
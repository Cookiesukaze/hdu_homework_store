@mouseover.native="mouseEnter()"
@mouseleave.native="mouseLeave()"
@row-click="tableRowClick" 在ref中添加以上内容获得滚动效果
<template>
    <div class="transparent-table">
        <el-table
                ref="datalist"
                class="tableBox"
                height="565"
                :row-style="{height: '0'}"
                :cell-style="{'text-align': 'center'}"
                :header-cell-style="{'text-align':'center','border':'solid 1px #E4E7EF',height:'1px',color: '#000000',  fontSize: '16px',background: 'rgba(249, 249, 249)'}"
                :data=computedData
                :show-header="true"
                :stripe="true"
                style="width: 110%;background-color:transparent">
            <el-table-column
                    prop="name"
                    label="客户名"
                    width="120">
                <template slot-scope="scope">
                    <div style="'text-align': 'center'">
                        <div class="table-text">{{scope.row.name}}</div>

                    </div>
                </template>
            </el-table-column>
            <el-table-column

                    prop="level"
                    label="最新外部等级"
                    width="120">
                <template slot-scope="scope">
                    <div style="'text-align': 'center'">
                        <div class="table-text" style="margin-top:1px; ">{{scope.row.level}}</div>
                    </div>
                </template>
            </el-table-column>
            <el-table-column
                    prop="status"
                    label="状态"
                    width="100">
                <template slot-scope="scope">
                    <div style="'text-align': 'center'">
                        <div class="table-text" style="margin-top:1px; ">{{scope.row.status}}</div>
                    </div>
                </template>
            </el-table-column>
            <el-table-column
                    prop="verifyStatus"
                    label="审核状态"
                    width="100">
                <template slot-scope="scope">
                    <div style="'text-align': 'center'">
                        <div class="table-text" style="margin-top:1px; ">{{scope.row.verifyStatus}}</div>
                    </div>
                </template>
            </el-table-column>
            <el-table-column
                    prop="violateReason"
                    label="认定违约原因"
                    width="200">
                <template slot-scope="scope">
                    <div style="'text-align': 'center'">
                        <div class="table-text" style="margin-top:1px; ">{{scope.row.violateReason}}</div>
                    </div>
                </template>
            </el-table-column>
            <el-table-column
                    prop="violateGravity"
                    label="严重程度"
                    width="120">
                <template slot-scope="scope">
                    <div style="'text-align': 'center'">
                        <div class="table-text" style="margin-top:1px; ">{{scope.row.violateGravity}}</div>
                    </div>
                </template>
            </el-table-column>
            <el-table-column
                    prop="confirmedPerson"
                    label="认定人"
                    width="100">
                <template slot-scope="scope">
                    <div style="'text-align': 'center'">
                        <div class="table-text" style="margin-top:1px; ">{{scope.row.confirmedPerson}}</div>
                    </div>
                </template>
            </el-table-column>
            <el-table-column
                    prop="applyTime"
                    label="认定申请时间"
                    width="150">
                <template slot-scope="scope">
                    <div style="'text-align': 'center'">
                        <div class="table-text" style="margin-top:1px; ">{{scope.row.applyTime}}</div>
                    </div>
                </template>
            </el-table-column>
            <el-table-column
                    prop="verifyTime"
                    label="认定审核时间"
                    width="120">
                <template slot-scope="scope">
                    <div style="'text-align': 'center'">
                        <div class="table-text" style="margin-top:1px; ">{{scope.row.verifyTime}}</div>
                    </div>
                </template>
            </el-table-column>
            <el-table-column
                    label="操作"
                    width="150">
                <template slot-scope="scope">
                    <div style="'text-align': 'center'">
                        <!--                        <router-link :to="{ path:'./violateapply',query:{name:'x'}}">查看附件</router-link>-->
                        <el-button
                                type="text"
                                @click="handleRespawn(scope.$index, scope.row)"
                        >重生审核</el-button>
                    </div>
                </template>

            </el-table-column>
        </el-table>

    </div>
</template>

<script>
//接口完成
import axios from 'axios';
var rolltimer=""

export default {
    props: {
        searchValue:{
            type:String,
            default(){
                return []
            },
        },
        searchCondition:{
            type:String,
            default(){
                return []
            },
        }
    },
    data() {
        return {
            mySearchValue:'',
            mySearchCondition:'',
            rollPx:1,
            rolltime:150,
            //掌握欠缺知识点名称和复习次数
            VQ_table_data_2: [
            {name:"",level:"",status:"",verifyStatus:"",violateReason:"",violateGravity:"",confirmedPerson:"",applyTime:"",verifyTime:"",},
                {name:"",level:"",status:"",verifyStatus:"",violateReason:"",violateGravity:"",confirmedPerson:"",applyTime:"",verifyTime:"",},
                {name:"",level:"",status:"",verifyStatus:"",violateReason:"",violateGravity:"",confirmedPerson:"",applyTime:"",verifyTime:"",},
                {name:"",level:"",status:"",verifyStatus:"",violateReason:"",violateGravity:"",confirmedPerson:"",applyTime:"",verifyTime:"",},
                {name:"",level:"",status:"",verifyStatus:"",violateReason:"",violateGravity:"",confirmedPerson:"",applyTime:"",verifyTime:"",},
                {name:"",level:"",status:"",verifyStatus:"",violateReason:"",violateGravity:"",confirmedPerson:"",applyTime:"",verifyTime:"",},
                {name:"",level:"",status:"",verifyStatus:"",violateReason:"",violateGravity:"",confirmedPerson:"",applyTime:"",verifyTime:"",},
                {name:"",level:"",status:"",verifyStatus:"",violateReason:"",violateGravity:"",confirmedPerson:"",applyTime:"",verifyTime:"",},
                {name:"",level:"",status:"",verifyStatus:"",violateReason:"",violateGravity:"",confirmedPerson:"",applyTime:"",verifyTime:"",},
                {name:"",level:"",status:"",verifyStatus:"",violateReason:"",violateGravity:"",confirmedPerson:"",applyTime:"",verifyTime:"",},
            ],
        }
    },
    watch:{
        searchValue:function(val){
            this.mySearchValue = val;
            console.log("违约查询表格（子）组件接收到了搜索值："+this.mySearchValue);
        },
        searchCondition:function(val){
            this.mySearchCondition = val;
            console.log("违约查询表格（子）组件接收到了搜索条件："+this.mySearchCondition);
        },
    },
    computed:{
        computedData(){
            if(this.mySearchValue) {
                return this.VQ_table_data_2.filter(item =>{
                        return Object.keys(item).some(key => {
                            return String(item[key]).toLowerCase().indexOf(this.mySearchValue) > -1
                        })
                    }
                )
            }else return this.VQ_table_data_2
        }
    },
    mounted(){
        axios.get("http://localhost:8081/respawn/af",{
        }).then(res=>{
            console.log(res.data);
            for(var i=0;i<res.data.data.length;i++){
                this.VQ_table_data_2[i].name=res.data.data[i].name;
                this.VQ_table_data_2[i].level=res.data.data[i].level;
                this.VQ_table_data_2[i].status=res.data.data[i].defaultState;
                this.VQ_table_data_2[i].verifyStatus=res.data.data[i].verifyStatus;
                this.VQ_table_data_2[i].violateReason=res.data.data[i].violateReason;
                this.VQ_table_data_2[i].respawnReason=res.data.data[i].respawnReason;
                this.VQ_table_data_2[i].violateGravity=res.data.data[i].violateGravity;
                this.VQ_table_data_2[i].confirmedPerson=res.data.data[i].confirmedPerson;
                this.VQ_table_data_2[i].applyTime=res.data.data[i].applyTime;
                this.VQ_table_data_2[i].verifyTime=res.data.data[i].verifyTime;
            }
            this.load();
        })
    },
    methods:{

        handleRespawn(index, row) {
            //this.VQ_table_data_2.splice(index, 1)

            this.$router.push({
                name: 'RespawnVerify', // 要跳转的路径的 name,可在 router 文件夹下的 index.js 文件内找
                //params: params
                params: {
                    nameData:this.VQ_table_data_2[index].name
                    // name:'xxx'
                }
            })
            console.log("违约查询表格(2)组件发送了值："+this.VQ_table_data_2[index].name)
        },
        sleep(milliSeconds) {
            var startTime = new Date().getTime();
            while (new Date().getTime() < startTime + milliSeconds) {}//暂停一段时间 1000=1S。
        },
        autoRoll(stop){
            if(stop){
                clearInterval(rolltimer);
                return;
            }
            const table=this.$refs.datalist;
            const divData=table.bodyWrapper;
            rolltimer=setInterval(()=>{
                divData.scrollTop+=this.rollPx;
                if(divData.clientHeight+divData.scrollTop>=divData.scrollHeight){
                    this.sleep(2000);
                    divData.scrollTop=0;
                }
            },this.rolltime);
        },
        //鼠标进入 停止滚动
        mouseEnter(time){
            this.autoRoll(true);
        },
        //鼠标离开 开始滚动
        mouseLeave(){
            this.autoRoll();
        },
    },
}
</script>

<style scoped>
.el-table::before {
    left: 0;
    bottom: 0;
    width: 100%;
    height: 0px;
}
/*.transparent-table /deep/ .el-table,*/
/*.transparent-table /deep/ .el-table__expanded-cell {*/
/*    background-color: transparent;*/
/*    color: white;*/
/*}*/
/*.transparent-table /deep/ .el-table th,*/
/*.transparent-table /deep/ .el-table tr,*/
/*.transparent-table /deep/ .el-table td {*/
/*    background-color: transparent !important;*/
/*    color: white;*/
/*}*/
/*.transparent-table /deep/.el-table__body-wrapper::-webkit-scrollbar{*/
/*    width: 0;*/
/*}*/
.transparent-table /deep/ .el-table__row td {
    /* 去除表格线 */
    border: none;
}
/*正常字体*/
.table-text{
    color: #000000;
    font-family: "opposans";
    font-style: normal;
    font-size: 15px;
    font-weight: normal;
    margin-top: 3px;
}
/*描述字体1*/
.table-text-1{
    color: #4F4F4F;
    font-family: "opposans";
    font-style: normal;
    font-size: 12px;
    font-weight: normal;
    margin-top: 1px;
}
/*描述字体2*/
.table-text-2{
    color: #000000;
    font-family: "opposans";
    font-style: normal;
    font-size: 12px;
    font-weight: normal;
    margin-top: 1px;
}

.table-circle{
    margin-top:3px;
}

</style>
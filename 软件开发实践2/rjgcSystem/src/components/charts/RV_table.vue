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
                width="200">
                <template slot-scope="scope">
                    <div style="'text-align': 'center'">
                        <div class="table-text">{{scope.row.name}}</div>
                    </div>
                </template>
            </el-table-column>
            <el-table-column
                prop="level"
                label="最新外部等级"
                width="150">
                <template slot-scope="scope">
                    <div style="'text-align': 'center'">
                        <div class="table-text" style="margin-top:1px;">{{scope.row.level}}</div>
                    </div>
                </template>
            </el-table-column>
            <el-table-column
                prop="status"
                label="状态"
                width="100">
                <template slot-scope="scope">
                    <div style="'text-align': 'center'">
                        <div class="table-text" style="margin-top:1px;">{{scope.row.status}}</div>
                    </div>
                </template>
            </el-table-column>
            <el-table-column
                prop="verifyStatus"
                label="审核状态"
                width="100">
                <template slot-scope="scope">
                    <div style="'text-align': 'center'">
                        <div class="table-text" style="margin-top:1px;">{{scope.row.verifyStatus}}</div>
                    </div>
                </template>
            </el-table-column>
            <el-table-column
                prop="violateReason"
                label="认定违约原因"
                width="200">
                <template slot-scope="scope">
                    <div style="'text-align': 'center'">
                        <div class="table-text" style="margin-top:1px;">{{scope.row.violateReason}}</div>
                    </div>
                </template>
            </el-table-column>
            <el-table-column
                prop="respawnReason"
                label="重生原因"
                width="200">
                <template slot-scope="scope">
                    <div style="'text-align': 'center'">
                        <div class="table-text" style="margin-top:1px;">{{scope.row.respawnReason}}</div>
                    </div>
                </template>
            </el-table-column>
            <el-table-column
                prop="confirmedPerson"
                label="认定人"
                width="100">
                <template slot-scope="scope">
                    <div style="'text-align': 'center'">
                        <div class="table-text" style="margin-top:1px;">{{scope.row.confirmedPerson}}</div>
                    </div>
                </template>
            </el-table-column>
            <el-table-column
                prop="applyTime"
                label="认定申请时间"
                width="120">
                <template slot-scope="scope">
                    <div style="'text-align': 'center'">
                        <div class="table-text" style="margin-top:1px;">{{scope.row.applyTime}}</div>
                    </div>
                </template>
            </el-table-column>
            <el-table-column
                label="操作"
                width="150">
                <template slot-scope="scope">
                    <div style="'text-align': 'center'">
                        <el-button
                            style="color:#e10000"
                            type="text"
                            @click="handleReject(scope.$index, scope.row)"
                        >拒绝</el-button>
                        <el-button
                            type="text"
                            @click="handleApply(scope.$index, scope.row)"
                        >同意</el-button>
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
            RV_table_data: [
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
            ]
        }
    },
    watch:{
        searchValue:function(val){
            this.mySearchValue = val;
            console.log("重生审核表格（子）组件接收到了搜索值："+this.mySearchValue);
        },
        searchCondition:function(val){
            this.mySearchCondition = val;
            console.log("重生审核表格（子）组件接收到了搜索条件："+this.mySearchCondition);
        }
    },
    computed:{
        computedData(){
            if(this.mySearchValue) {
                return this.RV_table_data.filter(item =>{
                        return Object.keys(item).some(key => {
                            return String(item[key]).toLowerCase().indexOf(this.mySearchValue) > -1
                        })
                    }
                )
            }else return this.RV_table_data
        }
    },
    mounted(){
        axios.get("http://localhost:8081/respawn/af",{
        }).then(res=>{
            console.log(res.data);
            for(var i=0;i<res.data.data.length;i++){
                this.RV_table_data[i].name=res.data.data[i].name;
                this.RV_table_data[i].level=res.data.data[i].level;
                this.RV_table_data[i].status=res.data.data[i].status;
                this.RV_table_data[i].verifyStatus=res.data.data[i].verifyStatus;
                this.RV_table_data[i].violateReason=res.data.data[i].violateReason;
                this.RV_table_data[i].respawnReason=res.data.data[i].respawnReason;
                this.RV_table_data[i].violateGravity=res.data.data[i].violateGravity;
                this.RV_table_data[i].confirmedPerson=res.data.data[i].confirmedPerson;
                this.RV_table_data[i].applyTime=res.data.data[i].applyTime;
                this.RV_table_data[i].verifyTime=res.data.data[i].verifyTime;
            }
            this.load();
        })
    },
    methods:{
        handleReject(index, row) {
            console.log("重生审核表格拒绝了" + this.RV_table_data[index].name)
            axios.post("http://localhost:8081/respawn/reject",{
                name:this.RV_table_data[index].name,
            })
            this.RV_table_data.splice(index, 1)
            window.reload();
        },
        handleApply(index, row) {
            console.log("重生审核表格通过了" + this.RV_table_data[index].name)
            axios.post("http://localhost:8081/respawn/agree",{
                name:this.RV_table_data[index].name
            })
            this.RV_table_data.splice(index, 1)
            window.reload();
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
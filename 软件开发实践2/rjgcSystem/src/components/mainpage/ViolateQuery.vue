<template>
  <div style="position: absolute;min-width: 1330px;margin-left: 10px;overflow-y: hidden;margin-top:-3px;">

        <div class="space_15"></div>
        <a-input-search
                v-model="searchValue"
              placeholder="请输入客户信息"
              enter-button="查询"
              size="large"
              @search="onSearch"
              style="margin-top:80px;margin-left: 80px;width: 800px"
        />
      <a-select v-model="searchCondition" @change="changeEvent()" size="large" placeholder="查询条件" style="width:200px;margin-top:80px;margin-left: 50px">
          <a-select-option value="name">客户名称</a-select-option>
          <a-select-option value="status">状态</a-select-option>
      </a-select>
        <VQ_table :searchValue="searchValue" :searchCondition="searchCondition" style="margin-left: 70px;margin-top:40px"></VQ_table>
        <div class="space_7"></div>

  </div>
</template>

<script>
//无接口
import VQ_table from "@/components/charts/VQ_table.vue";
export default {

    data(){
        return{
            searchCondition:'name',
            searchValue:"",
            region: undefined,
        }
    },
  components: {
      VQ_table,
    },
    methods: {
        onSearch(value) {
            this.searchValue = value
            console.log("违约查询组件获取了搜索值：" + this.searchValue)
        },
        changeEvent() {
            console.log("违约查询组件获取了搜索条件：" + this.searchCondition)
        },
    }
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
.space{
  margin-top: 10px; /*间距*/
}
.space_7{
  margin-top: 7px; /*间距*/
}
.space_15{
    margin-top: 15px; /*间距*/
}
::-webkit-scrollbar {
  width: 0 !important;
}
::-webkit-scrollbar {
  width: 0 !important;height: 0;
}

</style>
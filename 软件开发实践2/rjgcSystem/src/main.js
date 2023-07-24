import Vue from "vue";
import App from './App.vue';
import router from './router';
import store from './store';
import ElementUI from 'element-ui';
import 'element-ui/lib/theme-chalk/index.css';
import VueCalendarHeatmap from 'vue-calendar-heatmap';
import VXETable from 'vxe-table';
import 'vxe-table/lib/style.css';
import dataV from '@jiaminghi/data-view';
Vue.use(dataV);
Vue.use(ElementUI,VueCalendarHeatmap,VXETable);
import 'ant-design-vue/dist/antd.css';
import Antd from 'ant-design-vue';   //增加
import 'ant-design-vue/dist/antd.css';
Vue.use(Antd)  //Vue.use(Button)修改
// 在main.js中

import * as echarts from 'echarts';
//下边这两行尤为重要，数据才能正常渲染
import china from 'echarts/map/json/china.json';
echarts.registerMap('china', china);

Vue.prototype.$echarts = echarts;


//导入全局样式表
import './assets/css/global.css'

Vue.config.productionTip = false

//
import axios from 'axios'
import AntDirective from "ant-design-vue/lib/_util/antDirective";
//TODO：配置根路径
axios.defaults.baseURL="http://localhost:8080/"
Vue.prototype.$http=axios

new Vue({
  el: "#app",
  router,
  store,
  render: h => h(App)
}).$mount('#app')

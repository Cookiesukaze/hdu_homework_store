<!--
1.添加vue组件
2.在其他组件中使用
3.在index.js中声明
-->
<template>
  <div id="app">
    <!-- 路由占位符 -->
    <router-view></router-view>
  </div>
</template>

<script>
//import HelloWorld from './components/HelloWorld.vue'
import { mapGetters } from 'vuex';

export default {
  name: 'app',
  computed: {
    computed: mapGetters(['allPosts']),
  },
  mounted() {
    this.$store.dispatch("getPosts");
  }


}
</script>

<style>
@import "~@/assets/font/font.css";
body{
  font-family: opposans;
}
</style>

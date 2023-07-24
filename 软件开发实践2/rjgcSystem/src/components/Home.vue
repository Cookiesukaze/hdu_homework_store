<template>

    <el-container class="home-container">
    <!-- 头部区域 -->
    <el-header height="44px" class="home-header">
      <div class="science_header_left">
        <!-- TODO:插入logo -->
        <!-- <img src="../"> -->
        <div style="display: flex;flex-direction: row">
          <el-image
              style="width: 34px; height: 36px;border-radius: 50%;margin-top:2px"
              :src="logoUrl"
              :fit=fill></el-image>
          <span style="font-size: 18px;margin-left:10px;margin-top:6px;font-family: opposans;color: #ffffff">菁華学习行为分析系统</span>
        </div>

      </div>

        <div class="header_right">
            
                <img :src=avatarUrl width="25" height="25" style="border-radius:50%;">
                <span class="home_header_name" prefix-icon="el-icon-arrow-down">{{username}}</span>
                <!-- TODO:向下跳转至其他页面未写  使用下拉菜单写-->
                <i class="el-icon-arrow-down"></i>
        </div>
    </el-header>
    <!-- 页面主体区域 -->
    <el-container class="rela_container">
        <!-- 侧边栏 -->
        <el-aside :width="isCollapse ? '64px' : '220px' " class="home-aside">
            <!-- 个性化区域 -->
            <div class="chara_box" >
                <!-- 头像 -->
                <div class="avatar_box">
                    <img :src="avatarUrl" style="height:100%; width: 100%; border-radius: 50%;">
                </div>
                <!-- 用户名 -->
                <div class="aside_name">
                    <span >{{username}}</span>
                </div>
                <!-- 账号管理 -->
                <div style="text-align:center">
                <a  href="/setting"  class="set_aside" ><div style="margin-top: 2px;">账号管理</div></a>
                </div>
            </div>
            <!-- 实现菜单栏收缩 -->
            <div class="toggle-button" @click="toggleCollapse">|||</div>
            <!-- 侧边栏菜单区域 -->
            <el-menu background-color="#f9f9f9" unique-opened :collapse="isCollapse" :collapse-transition="false" router="true">
                <!-- TODO:响应时变成蓝色 -->
                <el-menu-item index="course" class="home_el-menu-item">
                    <i class="el-icon-collection"></i>
                    <span slot="title">课程</span>
                </el-menu-item>    
                <el-menu-item index="assiatant" class="home_el-menu-item">
                    <i class="el-icon-notebook-2"></i>
                    <span slot="title">助教功能</span>
                </el-menu-item>
                <el-menu-item index="receive" class="home_el-menu-item">
                    <i class="el-icon-receiving"></i>
                    <span slot="title">收件箱</span>
                </el-menu-item>
                <el-menu-item index="cloudy" class="home_el-menu-item">
                    <i class="el-icon-cloudy"></i>
                    <span slot="title">云盘</span>
                </el-menu-item>
            </el-menu>
        </el-aside >
        <!-- 右侧内容主题 -->
        <el-main style="padding:30px" class="home-main">
            <!-- 路由占位符 -->
            <router-view></router-view>
        </el-main>
    </el-container>
    </el-container>

</template>


<script>

    import logoUrl from "@/assets/logo.png";
    import axios from "axios";

    export default{
        data() {
            return{
                //是否折叠-初始状态
              isCollapse: false,
              avatarUrl:null,
              username:"",
              logoUrl:logoUrl,
            }
      
    },
        methods:{
            //点击按钮切换菜单的折叠与展开
            toggleCollapse(){
                this.isCollapse = !this.isCollapse;
            }
        },
      mounted() {
        axios.get("http://43.139.140.34:8081/person",
            {params:{
                id:111948682
              }
            }).then(res=> {
          console.log(res.data)
          console.log(res.data.data.avatarUrl)
          this.avatarUrl=res.data.data.avatarUrl
          this.username=res.data.data.name
        })

      },

        }
    

</script>


<style>
.home-container{
    height:100%;
}
.home-header{
    z-index: 10;
    background-color: #4c4c4c;
    display: flex;
    justify-content: space-between;
    align-items: center;
    width: 100%;
}
.home-aside{
    background-color:#f9f9f9;
}
.home-main{
    background-color: #f2f4f7;
    
}
.rela_container{
    position:absolute;
    padding: 44px 200px 0px 150px;
    width:100%;
    height:100%;
    
}
.header_left{
    color: white;
    padding-left:60px ;

}
.header_right{
    display: flex;
    padding-right:30px ;
}
.home_header_name{
    color: #fff;
    padding-left:10px;
    padding-right:1px ;
    align-items: center;
    display: flex;

}
.el-icon-arrow-down{
  margin-top:-2px;
  margin-left: 4px;
    color: white;
    padding-top: 7px;
}
.chara_box{
    /*background-image: url("../assets/background.png");*/
    height: 180px;
}
.avatar_box{
    height:80px;
    width: 80px;
    border-radius: 50%;
    padding:15px 70px 10px 70px ;
}
.aside_name{
    align-items: center;
    display: flex;
    flex-direction: column;
    padding: 0px 0px 10px 0px;
}
.set_aside{
    display: flex;
    position: relative;
    top:0px;
    left: 36px;
    padding: 0px 40px;
    border-radius: 20px;
    border: 2px solid;
    text-align: center;
    height:25px;
    width: 65px; 
}
.toggle-button{
    background-color: #f9f9f9;
    font-size: 10px;
    line-height:24px;
    color: black;
    text-align: center;
    letter-spacing: 0.2em;
    /* 鼠标响应为小手 */
    cursor: pointer;
}
.home_el-menu-item{
    left: 35px;
}
</style>
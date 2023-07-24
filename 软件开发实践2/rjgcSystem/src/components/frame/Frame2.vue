<template>

    <el-container class="science-container">
        <!-- 头部区域 -->
        <el-header height="44px" class="science-header">
            <div class="science_header_left">
                <!-- TODO:插入logo -->
                <!-- <img src="../"> -->
                <div style="display: flex;flex-direction: row">
                    <el-image
                            style="width: 34px; height: 36px;border-radius: 50%;margin-top:2px"
                            :src="logoUrl"
                            :fit=fill></el-image>
                    <span style="font-size: 18px;margin-left:10px;margin-top:6px;font-family: opposans">违约客户管理系统</span>
                </div>

            </div>

            <div class="science_header_right">

                <el-image
                        style="width: 28px; height: 28px;border-radius: 50%;margin-top:0px"
                        :src="avatarUrl"
                        :fit=fill></el-image>
                <el-dropdown>
                    <span class="science_el-dropdown-link">
                        {{ username }}<i class="el-icon-arrow-down el-icon--right" style="color:#606266"></i>
                    </span>
                    <el-dropdown-menu slot="dropdown">
                        <el-dropdown-item>退出登录</el-dropdown-item>
                    </el-dropdown-menu>
                </el-dropdown>
            </div>
        </el-header>
        <!-- 页面主体区域 -->
        <el-container class="science_rela_container" >
            <!-- 侧边栏 -->
            <!--      学生端高度！！！！-->
            <el-aside class="science_aside" width="180px" height="100%" style="overflow-y:hidden;height: 94vh;">
                <!-- 个性化区域 -->
                <div class="science_chara_box" >
                    <!-- 头像 -->
                    <div class="science_img">
                        <img src="../../assets/course_logo.jpg" style="height:80px; border-radius: 10px;">
                    </div>
                    <!-- 用户名 -->
                    <div class="science_name">
                        <span style="margin-top:40px;font-size:18px">欢迎回来！</span>
                    </div>

                </div>
                <!-- <ui class="science_ui"> -->
                <el-col :span="24" class="science_el-col" style="margin-top:20px">

                    <el-menu active-text-color="#3770ff !important;" router="true" default-active="/violatequery" @select="handleSelect" >

                        <!-- </ui> -->
                        <!-- <ui class="science_ui" > -->
                        <hr class="science_hr" style="margin-top:30px">



                        <el-menu-item index="violatequery1" class="science_el-menu-item" style="padding-left: 30px;">
                            <i class="el-icon-menu"></i>
                            <span slot="title">违约查询(风控)</span>
                        </el-menu-item>
                        <el-menu-item index="violatestatistics" class="science_el-menu-item" style="padding-left: 30px;">
                            <i class="el-icon-edit"></i>
                            <span slot="title">违约统计</span>
                        </el-menu-item>
                        <el-menu-item index="violateverify" class="science_el-menu-item" style="padding-left: 30px;">
                            <i class="el-icon-trophy"></i>
                            <span slot="title">违约审核</span>
                        </el-menu-item>




                    </el-menu>
                </el-col>
                <!-- </ui   > -->
            </el-aside>
            <!-- 右侧内容主题 -->
            <el-main style="padding:0px;background-color: #f2f4f7" class="course_el-main" >

                <!-- 路由占位符 -->
                <div class="mainpage-background"></div>
                <router-view :key="$route.fullPath"></router-view>
            </el-main>
        </el-container>
    </el-container>

</template>

<script>
import axios from "axios";
import logoUrl from "../../assets/logo.png";
export default {
    data() {
        return {
            activeIndex: 'violatequery',
            avatarUrl:null,
            username:"",
            logoUrl:logoUrl,
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
    watch: {
        $route () {
            this.handleSelect(this.activeIndex)
        }},
    methods: {
        handleSelect (index) {
            this.activeIndex = index
        }
    }
}
</script>


<style>
.mainpage-background{
    background: #ffffff;
    width: 1400px;
    height: 750px;
    margin-left:30px;
    margin-top:15px;
    z-index: 0;
    position: absolute;
}
.el-aside {
    display: block;
}
.science-header{
    background-color: #fff;
    box-shadow: 0 0 10px #e7e8ea;
    display: flex;
    justify-content: space-between;
    align-items: center;
    width: 100%;
    z-index: 10;
}
.science_header_left{
    padding-left:5px ;

}
.science_header_right{
    display: flex;
    padding-right:30px ;

}

.science_el-dropdown-link{
    margin-left: 5px;
    text-align: center;
    display: flex;
    cursor: pointer;
    font-size: 16px;
    align-items: flex-end

}
.science_chara_box{
    height:130px;
}
.science_img{

    padding: 25px 20px 10px 20px;
    height:78px;
    width:138px;
    border-radius:10px ;
}
.science_name{
    display: flex;
    justify-content: center;
}
/* .science_el-menu-item{

} */
/* .science_el-col{
    padding: 10px 0px;
    border-bottom: 2px solid rgba(211, 211, 211, 0.16);


} */
.science_rela_container{
    display: flex;
    position: relative;
    width: 100%;
    height: 100%;
}
.science_hr{
    background-color: rgba(211, 211, 211, 0.359);
    border:none;
    height:1px;
}
.course_el-main{
    background-color:#f2f4f7;
}
</style>

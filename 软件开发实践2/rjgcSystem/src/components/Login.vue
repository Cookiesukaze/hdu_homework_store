<template>
  <div class="login_container">

    <!-- 头部区域 -->
    <img src="../assets/login_background.png" style="position:absolute;width: 35%;left:150px;top:115px;">
    <el-header height="44px" class="science-header" style="z-index: 100">
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

        </div>
      </el-header>
    <div ref="vantaRef" style="width:100%;height:100vh;z-index: -1;" ></div>

    <div class="login_box" style="background-color: rgb(255,255,255);margin-left:350px" >

      <!--登录表单页面  -->
      <el-form ref="loginFormRef" :model="loginForm" :rules="loginFormRules" label-width="0px" class="login_form">
        <h3 id="login_font">用户登录</h3>
        <!-- 用户名 -->
        <el-form-item prop="username">
          <el-input v-model="loginForm.username" prefix-icon="el-icon-mobile" placeholder="账号">
          </el-input>
        </el-form-item>
        <!-- 密码 -->
        <el-form-item prop="password">
          <el-input v-model="loginForm.password" prefix-icon="el-icon-lock" type="password" placeholder="密码">
          </el-input>
        </el-form-item>
        <!-- 验证码 -->
<!--        <el-form-item class="check">-->
<!--          <el-input v-model="loginForm.checking" prefix-icon="el-icon-s-check" placeholder="验证码">-->
<!--          </el-input>-->
<!--        </el-form-item>-->
<!--        <img src="../assets/ic1.jpg" style="position:absolute;width: 18%;height: 9%;left:305px;top:235px;">-->
        <!-- 按钮 -->
        <el-form-item class="button">
            {{loginMessage}}
          <el-button type="primary" @click="btnLogin" round >登录</el-button>
        </el-form-item>
      </el-form>
<!--      <div>-->
<!--        <router-link  to="register">新用户注册</router-link>-->
<!--      </div>-->

    </div>
  </div>

</template>



<script>
import * as THREE from 'three'
import Net from 'vanta/src/vanta.net'
import axios from "axios";
import logoUrl from "../assets/logo.png";

export default {
  data(){
    return{
        loginMessage:"",
      //这里是登录表单数据绑定对象
      logoUrl:logoUrl,
      loginForm:{
        username:'',
        password:'',
        checking:''
      },
      //这是表单的用户规则对象
      loginFormRules:{
        //验证用户名是否合法
        username:
            [
              { required: true, message: '请输入手机号！', trigger: 'blur' },
              {
                min:1,
                max:11,
                message: "手机号输入有误",
                trigger: "blur"
              }
            ],
        //验证密码是否合法
        password:
            [
                { required: true, message: '请输入密码！', trigger: 'blur' },
              {
                min:6,
                max:15,
                message: "长度在6到15个字符之间",
                trigger: "blur"
              }
            ]
      }
    }
  },
  mounted() {
      this.vantaEffect = Net({
        el: this.$refs.vantaRef,
        THREE: THREE
      })

      this.vantaEffect.setOptions({
        el: this.$refs.vantaRef,
        mouseControls: true,
        touchControls: true,
        gyroControls: false,
        minHeight: 400.00,
        minWidth: 400.00,
        scale: 1.00,
        scaleMobile: 1.00,
        color: '#0051ff',
        backgroundColor: '#ffffff',
        spacing: 20.00
      })

  },
  beforeDestroy() {
    if (this.vantaEffect) {
      this.vantaEffect.destroy()
    }
  },
  methods:{
    //表单登录预校验，其中vaidate函数中的用一个回调函数，第一个返回值valid是一个布尔值返回校验结果
    login(){
      this.$refs.loginFormRef.validate(valid=>{
        console.log(valid);
      });
    },
    //TODO：登录按钮逻辑
    async btnLogin(){
        axios.post('http://localhost:8081/user/violatequery', {//用post方法传 输入框输入的用户名和密码
            username: this.loginForm.username,
            password: this.loginForm.password,
        }).then((successResponse) => {//回调函数当post成功后执行
            if (successResponse.data.code === 200) {//如果后端返回的状态码是200
                this.$router.replace({//路由替换为index
                    path: '/violatequery'
                });
            }
            else this.loginMessage = "用户名或密码错误！"
        })
      // this.$router.replace({//路由替换为index
      //   path: '/violatequery'
      // });
      // window.location.href=''
    }

  }
}
</script>

<style lang="less" scoped>
.login_container{
  height: 100%;
  font-family: opposans;
}
.login_box{
  height:300px;
  width:500px;
  border-radius: 20px;
  position: absolute;
  left: 50%;
  top: 50%;
  transform: translate(-50%,-50%);
  box-shadow: 0 0px 20px #dedede;
  font-family: opposans;
}
.login_form{
  position:absolute;
  width: 100%;
  padding: 40px 100px;
  box-sizing: border-box;
  font-family: opposans;

}
#login_font{
  font-style:normal;
  font-family: "宋体";
  font-size: 22px;
}
.check{
  width: 50%;
  font-family: opposans;
}
.button{
  font-family: opposans;
}

a{
  position: relative;
  left: 102px;
  top:320px;
  text-decoration: none;
  color:#3770ff;
  font-family: opposans;
}
.router-link-active {
  text-decoration: none;
  color:#3770ff;
  font-family: opposans;
}
::v-deep .el-input__inner{
  border-color: #ababab;
  font-family: opposans;
}
.science-header{
  background-color: #fff;
  box-shadow: 0 0 10px #e7e8ea;
  display: flex;
  justify-content: space-between;
  align-items: center;
  width: 100%;
  z-index: 10;
  font-family: opposans;
}
</style>
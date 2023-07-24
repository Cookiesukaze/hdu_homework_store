<template>
    <div class="register_container">
        <div class="register_box">
            <router-link to="login">&lt 返回</router-link>
             <!--注册表单页面  -->
             <el-form   ref="registerFormRef" :model="registerForm" :rules="registerFormRules" label-width="0px" class="register_form">
                <h3 id="register_font">新用户注册</h3>
                <!-- 手机号 -->
                 <el-form-item prop="username">
                    <el-input v-model="registerForm.username" prefix-icon="el-icon-mobile" placeholder="手机号">
                    </el-input>
                </el-form-item>
                <!-- 密码 -->
                <el-form-item prop="password">
                    <el-input v-model="registerForm.password" prefix-icon="el-icon-unlock" type="password" placeholder="密码">
                    </el-input>
                </el-form-item>
                <!-- 确认密码 -->
                <el-form-item class="checkPassword">
                    <el-input v-model="registerForm.checkPassword" prefix-icon="el-icon-lock" type="password" placeholder="确认密码">
                    </el-input>
                </el-form-item>
                <!-- 按钮 -->
                <el-form-item class="button">
                    <el-button type="primary" round>注册</el-button>
                    <el-button  round @click="resetRegisterForm">重置</el-button>
                </el-form-item>
            </el-form>
        </div>
    </div>
</template>


<script>
    export default{
        data(){
        //TODO：密码上下一致
            var validatePass2 = (rule, value, callback) => {
          if (value === '') {
              callback(new Error('请再次输入密码'));
          } else if (value !== this.registerFormRules.checkPassword) {
              callback(new Error('两次输入密码不一致!'));
          } else {
              callback();
          }
            };
            return{
            //这里是注册表单数据绑定对象
            registerForm:{
                username:'',
                password:'',
                checkPassword:''

            },
            registerFormRules:{
                //验证手机号是否合法
                username:[
                {
                    min:11,
                    max:11, 
                    pattern:/^((0\d{2,3}-\d{7,8})|(1[34578]\d{9}))$/,
                    message: "手机号输入有误",
                    trigger: "blur"
                    }
                ],
                //验证密码是否符合规范
                password:[
                    {
                        reguire: true,
                        min:8,
                        max:16,
                        pattern:/^(?![\d]+$)(?![a-zA-Z]+$)(?![^\da-zA-Z]+$)([^\u4e00-\u9fa5\s]){8,16}$/,
                        message:"密码要求8-16位，至少包含数字，字母，字符两种元素",
                        triggrt:"blur",

                    }

                ],
                checkPassword:[
                    {
                        
                        message:"上下密码需要一致",
                        reigger:"blur",
                    }
                ]




                
            },


            }



        },
        methods:{
            //点击重置按钮
            resetRegisterForm(){
                this.$refs.registerFormRef.resetFields();
            }
            //TODO：注册按钮逻辑
        }
    }
       
</script>

<style>

.register_container{
    background-color:#F2F4F7;
    height: 100%;
}

.register_box{
    height: 550px;
    width:540px;
    background-color: #fff;
    border-radius: 20px;
    position: absolute;
    left: 50%;
    top: 50%;
    transform: translate(-50%,-50%);
    box-shadow: 0 0 10px #e7e8ea;
}

a{
    position: absolute;
    top:25px;
    left:38px;
    color: #91A0B5;
    text-decoration: none;
    z-index: 10;
}

.register_form{
    position:inherit;
    width: 100%;
    box-sizing: border-box;
    padding: 60px 80px;
}

#register_font{
    font-style:normal;
    font-family: "宋体";
    font-size: 22px;
}
.button{
    display:flex;
    justify-content:flex-end;
}
</style>
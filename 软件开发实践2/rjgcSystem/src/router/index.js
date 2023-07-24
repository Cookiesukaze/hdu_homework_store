import Vue from 'vue'
import VueRouter from 'vue-router'
import Login from '../components/Login.vue'
import Register from '../components/Register.vue'
import Frame from "@/components/frame/Frame.vue";
import ViolateQuery from "@/components/mainpage/ViolateQuery.vue";
import ViolateApply from "@/components/mainpage/ViolateApply.vue";
import RespawnApply from "@/components/mainpage/RespawnApply.vue";
import ViolateVerify from "@/components/mainpage/ViolateVerify.vue";
import RespawnVerify from "@/components/mainpage/RespawnVerify.vue";
import ViolateStatistics from "@/components/mainpage/ViolateStatistics.vue";
import ViolateQuery1 from "@/components/mainpage/ViolateQuery1.vue";
import Frame3 from "@/components/frame/Frame3.vue";
import Frame2 from "@/components/frame/Frame2.vue";
import Frame1 from "@/components/frame/Frame1.vue";
import ViolateQuery2 from "@/components/mainpage/ViolateQuery2.vue";
Vue.use(VueRouter)


const routes = [
  {
    path: '/login',
    name: 'Login',
    component: Login
  },

  //TODO:放开重定向
  {
    path: '/',
    redirect:'/login'
  },
  {
    path:'/register',
    name:'Register',
    component: Register
  },
  {
    path:'/frame',
    name:'Frame',
    component:Frame,
    children:[{
      path:'/violatequery',
      name:'ViolateQuery',
      component:ViolateQuery
    },{
      path:'/violatequery1',
      name:'ViolateQuery1',
      component:ViolateQuery1
    },{
      path:'/violatequery2',
      name:'ViolateQuery2',
      component:ViolateQuery2
    },{
      path:'/violatestatistics',
      name:'ViolateStatistics',
      component:ViolateStatistics
    },
    {
      path:'/violateapply',
      name:'ViolateApply',
      component:ViolateApply
    }, {
        path:'/respawnapply',
        name:'RespawnApply',
        component:RespawnApply
      },
      {
        path:'/respawnverify',
        name:'RespawnVerify',
        component:RespawnVerify
      },
      {
        path:'/violateverify',
        name:'ViolateVerify',
        component:ViolateVerify
      },]
  },
  // {
  //   path:'/frame1',
  //   name:'Frame1',
  //   component:Frame1,
  //   children:[{
  //     path:'/violatequery',
  //     name:'ViolateQuery',
  //     component:ViolateQuery
  //   },{
  //     path:'/violatestatistics',
  //     name:'ViolateStatistics',
  //     component:ViolateStatistics
  //   }, {
  //       path:'/violateapply',
  //       name:'ViolateApply',
  //       component:ViolateApply
  //     }, {
  //       path:'/respawnapply',
  //       name:'RespawnApply',
  //       component:RespawnApply
  //     }]
  // },
  // {
  //   path:'/frame2',
  //   name:'Frame2',
  //   component:Frame2,
  //   children:[{
  //     path:'/violatequery1',
  //     name:'ViolateQuery1',
  //     component:ViolateQuery1
  //   },{
  //     path:'/violatestatistics',
  //     name:'ViolateStatistics',
  //     component:ViolateStatistics
  //   }, {
  //       path:'/violateverify',
  //       name:'ViolateVerify',
  //       component:ViolateVerify
  //     },]
  // },
  // {
  //   path:'/frame3',
  //   name:'Frame3',
  //   component:Frame3,
  //   children:[{
  //     path:'/violatequery2',
  //     name:'ViolateQuery2',
  //     component:ViolateQuery1
  //   },{
  //     path:'/violatestatistics',
  //     name:'ViolateStatistics',
  //     component:ViolateStatistics
  //   }, {
  //     path:'/respawnverify',
  //     name:'RespawnVerify',
  //     component:RespawnVerify
  //   },]
  // },
]

const router = new VueRouter({
  mode:"hash",
  routes
})

export default router

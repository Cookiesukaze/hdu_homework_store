<template>
    <a-form
            id="components-form-demo-validate-other"
            :form="form"
            v-bind="formItemLayout"
            labelAlign="left"
            @submit="handleSubmit"
    >

        <a-form-item label="客户名称">
            <a-input
                    v-decorator="['name',
          { rules: [ { required: true,message: '必须输入客户名称！' },
          {min:2,max:30,message: '名称长度需为2~36之间!'},
          {pattern: /^[^\s]*$/,message: '禁止输入空格!'},
          {pattern: '^[\u4E00-\u9FA5A-Za-z0-9]+$',message: '禁止输入特殊字符!'}
          ] },]"
                    placeholder="请输入客户名称"/>
        </a-form-item>

        <a-form-item label="选择重生原因">
            <a-select
                    v-decorator="['respawnReason',
          {
            rules: [
              { required: true, message: '必须选择一条重生原因！'},
            ],
          },
        ]"
                    placeholder="请选择重生原因"
            >
                <a-select-option value="1">
                    正常结算后解除
                </a-select-option>
                <a-select-option value="2">
                    在其他金融机构违约解除，或外部评级显示为非违约级别
                </a-select-option>
                <a-select-option value="3">
                    计提比例小于设置界限
                </a-select-option>
                <a-select-option value="4">
                    连续 12 个月内按时支付本金和利息
                </a-select-option>
                <a-select-option value="5">
                    客户的还款意愿和还款能力明显好转，已偿付各项逾期本金、逾期利息和其他费用（包
                    括罚息等），且连续 12 个月内按时支付本金、利息
                </a-select-option>
                <a-select-option value="6">
                    导致违约的关联集团内其他发生违约的客户已经违约重生，解除关联成员的违约设定
                </a-select-option>
            </a-select>
        </a-form-item>

        <a-form-item label="选择违约原因">
            <a-select
                v-decorator="[
          'violateReason',
          {
          },
        ]"
                placeholder="请选择违约原因"
            >
                <a-select-option value="1">
                    6 个月内，交易对手技术性或资金等原因，给当天结算带来头寸缺口 2 次以上
                </a-select-option>
                <a-select-option value="2">
                    6 个月内因各种原因导致成交后撤单 2 次以上
                </a-select-option>
                <a-select-option value="3">
                    未能按照合约规定支付或延期支付利息，本金或其他交付义务
                </a-select-option>
                <a-select-option value="4">
                    关联违约
                </a-select-option>
                <a-select-option value="5">
                    发生消极债务置换
                </a-select-option>
                <a-select-option value="6">
                    申请破产保护，发生法律接管，或者处于类似的破产保护状态
                </a-select-option>
                <a-select-option value="7">
                    在其他金融机构违约，或外部评级显示为违约级别
                </a-select-option>
            </a-select>
        </a-form-item>

        <a-form-item label="最新外部等级">
            <a-slider
                v-decorator="['level']"
                :marks="{ 0: 'CCC', 16: 'B', 33: 'BB', 50: 'BBB', 67: 'A', 83: 'AA',100: 'AAA' }"
            />
        </a-form-item>

        <a-form-item label="违约严重性">
            <a-radio-group v-decorator="['violateGravity']">
                <a-radio-button value="1">
                    高
                </a-radio-button>
                <a-radio-button value="2">
                    中
                </a-radio-button>
                <a-radio-button value="3">
                    低
                </a-radio-button>
            </a-radio-group>
        </a-form-item>

        <a-form-item label="备注">
            <a-textarea
                v-decorator="['comment']"
                :value="value"
                placeholder="请输入备注" :rows="4" />
        </a-form-item>

        <a-form-item :wrapper-col="{ span: 12, offset: 6 }">
            <a-button type="primary" html-type="submit">
                认定重生
            </a-button>
            {{submitMessage}}
        </a-form-item>
    </a-form>
</template>

<script>
import axios from 'axios';
export default {
    props:["formData"],
    data: () => ({
        submitMessage:'',
        formItemLayout: {
            labelCol: { span: 6 },
            wrapperCol: { span: 14 },
        },
    }),
    beforeCreate() {
        this.form = this.$form.createForm(this, { name: 'validate_other' });
    },
    mounted(){
        //设置表单初始值
        this.form.setFieldsValue({
            name: this.checkValue(this.formData.name),
            violateReason: this.checkValue(this.formData.violateReason),
            violateGravity: this.checkValue(this.formData.violateGravity),
            level: this.checkValue(this.formData.level),
        })
    },
    methods: {
        checkSpecialKey(str) {
            let specialKey =
                "[`~!#$^&*()=|{}':;'\\[\\].<>/?~！#￥……&*（）——|{}【】‘；：”“'。，、？]‘'";
            for (let i = 0; i < str.length; i++) {
                if (specialKey.indexOf(str.substr(i, 1)) != -1) {
                    return false;
                }
            }
            return true;
        },
        checkValue(value){//判空
            if(value!='')return value;//也不能是null
            else return undefined;//不能是null
        },
        handleSubmit(e) {
            e.preventDefault();
            this.form.validateFields((err, values) => {
                if (!err) {
                    console.log('重生申请表单提交结果: ', values);
                    // console.log('重生申请表单提交结果: ', values.name);
                    if(values.level<16)values.level='CCC'
                    else if(values.level<33)values.level='B'
                    else if(values.level<50)values.level='BB'
                    else if(values.level<67)values.level='BBB'
                    else if(values.level<83)values.level='A'
                    else if(values.level<100)values.level='AA'
                    else if(values.level==100)values.level='AAA'
                    axios.post("http://localhost:8081/respawn/apply",{
                        name:values.name,
                        respawnReason:values.respawnReason,
                        violateReason:values.violateReason,
                        level:values.level,
                        violateGravity:values.violateGravity,
                        comment:values.comment,
                    }).then(res=>{
                        console.log(res)
                        if(res.data.msg=="查询不到该客户")
                        {
                            this.submitMessage = "  未查询到"+values.name+"客户 !";
                        }else{
                            this.submitMessage = "  已提交客户 "+values.name+" 的重生申请单 !";
                        }
                    })

                    }
                    
                });
            
        },
        normFile(e) {
            console.log('Upload event:', e);
            if (Array.isArray(e)) {
                return e;
            }
            return e && e.fileList;
        },
    },
};
</script>

<style>
#components-form-demo-validate-other .dropbox {
    height: 180px;
    line-height: 2.5;
}
</style>
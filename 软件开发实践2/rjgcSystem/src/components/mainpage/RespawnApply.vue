<template>
    <div style="position: absolute;min-width: 1330px;margin-left: 10px;overflow-y: hidden;margin-top:-3px;">

        <div class="space_15"></div>
        <RA_form :formData="formData" style="margin-left: 300px;margin-top:120px"></RA_form>
        <div class="space_7"></div>

    </div>
</template>

<script>
//无接口
import RA_form from "@/components/charts/RA_form.vue";
export default {
    data(){
        return{
            formData:this.$route.params.formData,
        }
    },
    components: {
        RA_form,
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
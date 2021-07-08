<template>
  <div class="overview" style="margin-left:20px">

    <div id="nav">
      <router-link to="/">
        <el-radio-group>
          <el-button id="button" to="/" icon="el-icon-s-home"></el-button>
        </el-radio-group>
      </router-link> 
    </div>

    <!-- plant overview
     <el-row :gutter="20">
      <el-col :span="5"><div class="grid">
        <h2>Gerthrude</h2>
        <router-link to="/overview/73231108178">
          <img src="../assets/plant1.png"/>
        </router-link>
        </div> 
      </el-col>
      <el-col :span="5"><div class="grid">
        <h2>Second Plant</h2>
        <img src="../assets/plant2.png"/>
        </div>
      </el-col>
      <el-col :span="5"><div class="grid">
        <h2>Third Plant</h2>
        <img src="../assets/plant3.png"/>
        </div>
      </el-col>
      <el-col :span="5"><div class="grid">
        <h2>Fourth Plant</h2>
        <img src="../assets/plant4.png"/>
        </div>
      </el-col>
    </el-row>
    <br><br> -->

    <!-- plant name -->
    <el-row>
      <el-col :span="14">
        <div class="grid">
            Name <br>
            <el-input placeholder="Gerthrude" v-model="name" :disabled="true" class="input1" size="medium"></el-input>
            <br><br>

            <!-- description -->
             Description <br>
            <el-input v-model="description" type="textarea" :rows="2" :disabled="true" placeholder="Hey, you are my new plant!" class="input2" size="medium">
            </el-input>
            <br>

            <!-- owner -->
            The owner of the plant is <br>
            <el-input v-model="owner" placeholder="Fabienne Vatter"  :disabled="true" class="input1"></el-input>
        </div>
      </el-col>

      <!-- picture -->
      <!-- <el-col :span="5">
        <div class="grid">
            Picture <br>
            <img style="margin-top:7px;" src="../assets/plant1.png"/>
            <br>
        </div>
      </el-col> -->

      <!-- color -->
      <el-col :span="2">
        <div class="grid">
            Colour <br>
            <el-color-picker :disabled="true" style="margin-top:7px" v-model="color1"></el-color-picker>
        </div>
      </el-col>
    </el-row>

    <!-- plant id -->
    <el-row>
        <el-col :span="12">
            <div class="grid">
            The plant id is <br>
            <el-input v-model="plantId" placeholder="21937834302" :disabled="true" class="input3" size="medium"></el-input>
            </div>
        </el-col>
    </el-row>

    <!-- growth -->
    <el-row>
        <el-col :span="12">
            <div class="grid">
            The plants growth is <br>
            <el-input v-model="age" placeholder="Sprout" :disabled="true" class="input3" size="medium"></el-input>
            </div>
        </el-col>
    </el-row>



    <!-- last watered -->
    <el-row>
        <el-col :span="4">
            <div class="grid">
            Last watered <br>
            <el-input v-model="lastWatered" placeholder="19.06.2021" :disabled="true" class="input4" size="medium"></el-input>
            </div>
        </el-col>
        <el-col :span="4">
            <div class="grid">
            <span style="color: white;">Last watered </span><br>
            <el-input placeholder="10:00 Uhr" :disabled="true" class="input5" size="medium"></el-input>
            </div>
        </el-col>
    </el-row>

    <!-- watering state -->
    <el-row>
        <el-col :span="12">
            <div class="grid">
            The plant watering state is <br>
            <el-input placeholder="5" :disabled="true" class="input3" size="medium"></el-input>
            </div>
        </el-col>
    </el-row>
  </div>
</template>

<script>
import axios from 'axios'
export default {
  async mounted() {
    console.log( this.$route.params.id)
    let config = {
    headers: {'Access-Control-Allow-Origin': '*'}
    };
      await axios 
        .get("http://localhost:3000/overview/" + this.$route.params.id, config)
        .then(res => { 
          console.log(res)
          this.plantId = this.$route.params.id;
          this.name = res.data.name;
          this.description = res.data.description;
          this.owner = res.data.owner;
          this.waterLevel = res.data.waterLevel;
          this.lastWatered = res.data.lastWatered;
        })
        .catch(error => (console.log(error)))
  },
  name: 'Overview',
  props: {
    msg: String
  },
  el: "...",
  data: function() {
    return {
      color1: '#409EFF',
      name:"",
      description:"",
      owner:"",
      plantId:"",
      lastWatered:"",
      waterLevel:"",
    }
  },
  methods: {

  }
}
</script>

<style scoped>
.el-row {
    margin-bottom: 20px;
}
.el-col {
  border-radius: 4px;
}
.grid {
  border-radius: 4px;
  min-height: 36px;
  background: #ffffff;
}
img {
    height: 200px;
}
h1 {
  font-size: 40px;
}
.input1 {
  margin-top:7px;
  width:87%;
  margin-right:40px;
}
.input2 {
  margin-top:7px; 
  width:87%; 
  margin-right:40px; 
  margin-bottom:20px
}
.input3 {
  margin-top:7px; 
  width:30%; 
  margin-right:30px;
}
.input4 {
  margin-top:7px; 
  width:75%; 
  margin-right:40px
}
.input5 {
  margin-top:7px; 
  width:70%; 
  margin-right:40px
}
</style>




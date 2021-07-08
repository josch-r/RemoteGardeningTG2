<template>
  <div class="add" style="margin-left:20px">

    <div id="nav">
      <router-link to="/">
        <el-radio-group v-model="radio1">
          <el-button id="button" to="/" icon="el-icon-s-home"></el-button>
        </el-radio-group>
      </router-link> 
    </div>

    <!-- input name plant -->
    Put the name of your plant here <br>
    <el-input v-model="nameInput" class="input1"></el-input>
    <br><br>

    <!-- select owner -->
    <el-row>
      <el-col :span="8">
        <div class="grid">
          The owner of the plant is <br>
          <el-select v-model="ownerInput" placeholder="Please select one" style="margin-top:7px;">
            <el-option
              v-for="item in options"
              :key="item.ownerInput"
              :label="item.label"
              :value="item.ownerInput">
            </el-option>
          </el-select>
        </div>
      </el-col>

      <!-- plant id -->
      <el-col :span="12">
        <div class="grid">
          The plant id is <br>
          <el-input v-model="plantId" :disabled="true" class="input2" size="medium">
          </el-input>
        </div>
      </el-col>
    </el-row>


    <!-- add picture/icon -->
    <el-row>
      <el-col :span="8">
        <div class="grid">
          Add a picture or icon <br>
          <el-upload action="https://jsonplaceholder.typicode.com/posts/" style="margin-top:7px;" list-type="picture-card" :on-preview="handlePictureCardPreview" :on-remove="handleRemove">
            <i class="el-icon-picture-outline"></i>
          </el-upload>
          <el-dialog v-model="dialogVisible">
            <img width="100%" :src="dialogImageUrl" alt=""/>
          </el-dialog>
          <br><br>
        </div>
      </el-col>

      <!-- add a color -->
      <el-col :span="12">
        <div class="grid">
          Add a color <br>
          <el-color-picker v-model="color" style="margin-top:7px;"></el-color-picker>
        </div>
      </el-col>
    </el-row>

    <!-- add description -->
    Add a description to your plant <br>
    <el-input type="textarea" :rows="2" v-model="descriptionInput" class="input3" size="medium">
    </el-input>
    <br>

    <!-- !!!!!!!!chose waterlevel -->
    Choose waterlevel <br>
    <el-tooltip class="item" effect="light" content="once per week" placement="bottom" style="margin-top:7px;">
      <el-button icon="el-icon-heavy-rain" circle v-on:click="waterLevel = 6"></el-button>
    </el-tooltip>
    <el-tooltip class="item" effect="light" content="twice per week" placement="bottom">
      <el-button icon="el-icon-heavy-rain" circle v-on:click="waterLevel = 5"></el-button>
    </el-tooltip>
    <el-tooltip class="item" effect="light" content="three times a week" placement="bottom">
      <el-button icon="el-icon-heavy-rain" circle v-on:click="waterLevel = 4"></el-button>
    </el-tooltip>
    <el-tooltip class="item" effect="light" content="four times a week" placement="bottom">
      <el-button icon="el-icon-heavy-rain" circle v-on:click="waterLevel = 3"></el-button>
    </el-tooltip>
    <el-tooltip class="item" effect="light" content="everyday" placement="bottom">
      <el-button icon="el-icon-heavy-rain" circle v-on:click="waterLevel = 2"></el-button>
    </el-tooltip>
    <el-tooltip class="item" effect="light" content="twice per day" placement="bottom">
      <el-button icon="el-icon-heavy-rain" circle v-on:click="waterLevel = 1"></el-button>
    </el-tooltip>
    <br><br><br>

    <!-- chose health -->
    Choose health <br>
    <el-slider 
      style="margin-left:12px; margin-top:7px;" 
      v-model="healthState"
      :marks="marks"
      :step="25"
      show-input>
    </el-slider>
    <br>

    <!-- chose disease -->
    <el-radio-group v-model="disease" style="margin-top:17px; margin-left:5px">
      <el-radio :label="1">no disease</el-radio>
      <el-radio :label="2">dried up leaves</el-radio>
      <el-radio :label="3">Mosquitoes</el-radio>
      <el-radio :label="4">overwatered</el-radio>
      <el-radio :label="5">brown corners</el-radio>
      <el-radio :label="6">plant disease</el-radio>
      <el-radio :label="7">rots</el-radio>
    </el-radio-group>
    <br><br><br>

    <!-- choose growth -->
    Choose growth <br>
    <el-select v-model="growth" placeholder="Please select one" style="margin-top:7px;">
      <el-option
        v-for="item in options2"
        :key="item.growth"
        :label="item.label"
        :value="item.growth">
      </el-option>
    </el-select>
    <br><br><br>
    
    <!-- save plant to garden -->
    <router-link to="/">
      <el-button id="button" v-on:click="generatePlant()">Save plant to your garden!</el-button>
    </router-link>
    <br><br><br>

  </div>
</template>

<script>
import IconName from '@carbon/icons/es/sprout/16';
import { ref } from 'vue'
import axios from 'axios';
export default {
  name: 'Add',
  props: {
    msg: String
  },
  el: "...",
  data: function() {
    return {
      nameInput: ref(""),
      disease: 9,
      options: [{
          ownerInput: 'Jannes Blobel',
          label: 'Jannes Blobel'
        }, {
          ownerInput: 'Joschua Rothenbacher',
          label: 'Joschua Rothenbacher'
        }, {
          ownerInput: 'Fabienne Vatter',
          label: 'Fabienne Vatter'
        }],
        ownerInput: '',
      descriptionInput: ref(' '),
      dialogImageUrl: '',
      dialogVisible: false,
      value1: null,
      value2: null,
      icon1: IconName,
      healthState: 0,
      marks: {
        0: 'dead',
        25: 'unhealthy',
        50: 'ok',
        75: 'good',
        100: 'healthy'
      },
      options2: [{
          growth: 'Seeds',
          label: 'Seeds'
        }, {
          growth: 'Sprout',
          label: 'Sprout'
        }, {
          growth: 'under 20cm',
          label: 'under 20cm'
        }, {
          growth: '20-40cm',
          label: '20-40cm'
        }, {
          growth: 'mature',
          label: 'mature'
      }],
      growth: '',
      color: '#4BD8C3',
      selectedFile: null,
      plantId: "",
      waterLevel: "",
    }
  },
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
        })
        .catch(error => (console.log(error)))
    },
  methods: {
    handleRemove(file, fileList) {
      console.log(file, fileList)
    },
    handlePictureCardPreview(file) {
        this.dialogImageUrl = file.url
        this.dialogVisible = true
    },
    
    async generatePlant() {
      let plant = {
        rfidUID: this.plantId,
        name: this.nameInput,
        owner: this.ownerInput,
        waterLevel: this.waterLevel,
        color: this.color1,
        description: this.descriptionInput,
        health: this.healthState,
        disease: this.disease,
        growth: this.growth,
      }
      console.log(plant);
      axios
        .post("http://localhost:3000/overview/update", plant)
        .then(response => (console.log(response)))
        .catch(error => (console.log(error)))
    },
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
  width: 200px;
}
h1 {
  font-size: 40px;
}
.input1 {
  margin-top:7px; 
  width:50%; 
  margin-right:40px; 
  margin-bottom:18px
}
.input2 {
  margin-top:7px; 
  width:30%; 
  margin-right:40px; 
  margin-bottom:20px
}
.input3 {
  margin-top:7px; 
  width:50%; 
  margin-right:40px; 
  margin-bottom:20px
}
</style>


<template>
  <div class="add" style="margin-left:20px">

    <div id="nav">
      <router-link to="/">
        <el-radio-group v-model="radio1">
          <el-button to="/" icon="el-icon-s-home"></el-button>
        </el-radio-group>
      </router-link> 
    </div>

    <!-- input name plant -->
    Put the name of your plant here <br>
    <el-input placeholder="name of your plant" v-model="input" style="margin-top:7px; width:50%; margin-right:40px; margin-bottom:20px" size="medium"></el-input>
    <br><br>

    <!-- select owner -->
    <el-row>
      <el-col :span="8">
        <div class="grid-content bg-white">
          The owner of the plant is <br>
          <el-select v-model="value" placeholder="Select" style="margin-top:7px;">
            <el-option
              v-for="item in options"
              :key="item.value"
              :label="item.label"
              :value="item.value">
            </el-option>
          </el-select>
        </div>
      </el-col>

      <!-- owner id -->
      <el-col :span="12">
        <div class="grid-content bg-white">
          The plant id is <br>
          <el-input placeholder="owner id" :disabled="true" style="margin-top:7px; width:20%; margin-right:40px; margin-bottom:20px" size="medium">
          </el-input>
        </div>
      </el-col>
    </el-row>


    <!-- add picture/icon -->
    <el-row>
      <el-col :span="8">
        <div class="grid-content bg-white">
          Add a picture or icon <br>
          <el-upload action="https://jsonplaceholder.typicode.com/posts/" style="margin-top:7px;" list-type="picture-card" :on-preview="handlePictureCardPreview" :on-remove="handleRemove">
            <i class="el-icon-picture-outline"></i>
          </el-upload>
          <el-dialog v-model="dialogVisible">
            <img width="100%" :src="dialogImageUrl" alt="" />
          </el-dialog>
          <br><br>
        </div>
      </el-col>

      <!-- add a color -->
      <el-col :span="12">
        <div class="grid-content bg-white">
          Add a color <br>
          <el-color-picker v-model="color1" style="margin-top:7px;"></el-color-picker>
        </div>
      </el-col>
    </el-row>

    <!-- add description -->
    Add a description to your plant <br>
    <el-input type="textarea" :rows="2" placeholder="Please input a description" v-model="textarea" style="margin-top:7px; width:50%; margin-right:40px; margin-bottom:20px" size="medium">
    </el-input>
    <br>

    <!-- chose waterlevel -->
    Choose waterlevel <br>
    <el-tooltip class="item" effect="light" content="once per week" placement="bottom" style="margin-top:7px;">
      <el-button icon="el-icon-heavy-rain" circle></el-button>
    </el-tooltip>
    <el-tooltip class="item" effect="light" content="twice per week" placement="bottom">
      <el-button type="secondary" icon="el-icon-heavy-rain" circle></el-button>
    </el-tooltip>
    <el-tooltip class="item" effect="light" content="three times a week" placement="bottom">
      <el-button type="secondary" icon="el-icon-heavy-rain" circle></el-button>
    </el-tooltip>
    <el-tooltip class="item" effect="light" content="four times a week" placement="bottom">
      <el-button type="secondary" icon="el-icon-heavy-rain" circle></el-button>
    </el-tooltip>
    <el-tooltip class="item" effect="light" content="everyday" placement="bottom">
      <el-button type="secondary" icon="el-icon-heavy-rain" circle></el-button>
    </el-tooltip>
    <el-tooltip class="item" effect="light" content="twice per day" placement="bottom">
      <el-button type="secondary" icon="el-icon-heavy-rain" circle></el-button>
    </el-tooltip>
    <br><br><br>

    <!-- chose health -->
    Choose health <br>
    <el-slider 
      style="margin-left:12px; margin-top:7px;" 
      v-model="value3"
      :marks="marks"
      :step="25"
      show-input>
    </el-slider>
    <br><br>
  
    <el-radio-group v-model="radio" style="margin-left:4px">
      <el-radio :label="1">dried up leaves</el-radio>
      <el-radio :label="2">Mosquitoes</el-radio>
      <el-radio :label="3">overwatered</el-radio>
      <el-radio :label="4">brown corners</el-radio>
      <el-radio :label="5">plant disease</el-radio>
      <el-radio :label="7">rots</el-radio>
    </el-radio-group>
    <br><br><br>

    <!-- chose age -->
    Choose age <br>
    <el-select v-model="val" placeholder="Select" style="margin-top:7px;">
      <el-option
        v-for="item in options2"
        :key="item.val"
        :label="item.label"
        :value="item.val">
      </el-option>
    </el-select>
    <br><br><br>
    
    <!-- save plant to garden -->
    <router-link to="/">
      <el-button plain v-on:click="save()">Save your plant to your garden!</el-button>
    </router-link>
    <br><br><br>

  </div>
</template>

<script>
import IconName from '@carbon/icons/es/sprout/16';
import { ref } from 'vue'
export default {
  name: 'Add',
  props: {
    msg: String
  },
  el: "...",
  data: function() {
    return {
      radio1: 'Home',
      radio2: 'Add new',
      radio: 8,
      options: [{
          value: 'Jannes Blobel',
          label: 'Jannes Blobel'
        }, {
          value: 'Joschua Rothenbacher',
          label: 'Joschua Rothenbacher'
        }, {
          value: 'Fabienne Vatter',
          label: 'Fabienne Vatter'
        }],
        value: '',
      input: ref(' '),
      textarea: ref(' '),
      dialogImageUrl: '',
      dialogVisible: false,
      value1: null,
      value2: null,
      colors1: ['grey', 'lightblue', 'blue'],
      colors2: ['grey', 'orange', 'red'],
      iconClasses1: ['el-icon-heavy-rain', 'el-icon-heavy-rain', 'el-icon-heavy-rain'],
      iconClasses2: ['icon-rate-face-1', 'icon-rate-face-2', 'icon-rate-face-3'],
      icon1: IconName,
      value3: 0,
      marks: {
        0: 'dead',
        25: 'unhealthy',
        50: 'ok',
        75: 'good',
        100: 'healthy'
      },
      options2: [{
          val: 'Seeds',
          label: 'Seeds'
        }, {
          val: 'Sprout',
          label: 'Sprout'
        }, {
          val: 'under 20cm',
          label: 'under 20cm'
        }, {
          val: '20-40cm',
          label: '20-40cm'
        }, {
          val: 'mature',
          label: 'mature'
      }],
      val: '',
      color1: '#409EFF',
      selectedFile: null,
    }
  },
  methods: {
    handleRemove(file, fileList) {
      console.log(file, fileList)
    },
    handlePictureCardPreview(file) {
        this.dialogImageUrl = file.url
        this.dialogVisible = true
    },
  }
}
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
.el-row {
    margin-bottom: 20px;
    &:last-child {
      margin-bottom: 0;
    }
}
.el-col {
  border-radius: 4px;
}
.bg-purple-dark {
  background: #99a9bf;
}
.grid-content {
  border-radius: 4px;
  min-height: 36px;
}


.el-row {
  margin-bottom: 20px;
}
.el-col {
  border-radius: 4px;
}
.bg-white {
  background: #ffffff;
}
.grid2 {
  border-radius: 4px;
  min-height: 200px;
}
button {
    padding: 8px 20px;
    font-size: 17px;
    text-align: center;
    cursor: pointer;
    outline: none;
    color: #fff;
    background-color:#0b380d;
    border: none;
    border-radius: 15px;
    box-shadow: 0 6px #82daa7;
}
button:hover {
    background-color: #0b380d;
}
button:active {
  background-color: #0b380d;
  box-shadow: 0 0px;
  transform: translateY(4px);
}
img {
  width: 200px;
}
h1 {
  font-size: 40px;
}
</style>


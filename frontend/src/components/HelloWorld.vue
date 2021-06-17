<template>
  <div class="hello">
    <h1>{{ msg }}</h1>

    <!-- weather panel -->
    <el-row :gutter="20">
      <el-col :span="10" :offset="7">
        <div class="grid1 bg-green">
          <h2>Weather</h2>
          24°C <br> RAIN AND SUN
        </div>
      </el-col>
    </el-row>

    <!-- carousel garden -->
    <el-row :gutter="20">
      <el-col :span="16" :offset="4">
        <div class="grid2 bg-green">
          <el-carousel type="card" trigger="click" height="200px" width="50px">
            <el-carousel-item v-for="item in pics" :key="item">
              <img :src="item">
            </el-carousel-item>
          </el-carousel>
        </div>
      </el-col> 
    </el-row>

    <!-- Add new plant -->
    <el-button-group>
      <router-link to="/add">
        <el-button type="primary" icon="el-icon-plus"> Add Plant!</el-button>
      </router-link>
    </el-button-group>
  
    <br>
    <br>
    <br>

    <!-- calendar -->
    <el-row :gutter="20">
      <el-col :span="16" :offset="4">
        <div class="grid2 bg-green">
          <el-calendar v-model="value">
            <template #dateCell="{ data }">
              <p :class="data.isSelected ? 'is-selected' : ''">
                {{ data.day.split('-').slice(2).join('-') }} {{ data.isSelected ? '✔️' : '' }}
              </p>
            </template>
          </el-calendar>
        </div>
      </el-col>
    </el-row>

    <!-- notification panel -->
    <el-row :gutter="20">
      <el-col :span="16" :offset="4">
        <div class="grid1 bg-green">
          <span class="one">Water your plant Freddy today! He is thirsty!</span>
          <el-checkbox v-model="checked"></el-checkbox>
        </div>
      </el-col>
    </el-row>

    <!-- water plant of someone else -->
    <el-dropdown>
      <span class="el-dropdown-link">
        <h2>Water plant of...<i class="el-icon-arrow-down el-icon--right"></i></h2>
      </span>
      <template #dropdown>
        <el-dropdown-menu>
          <el-dropdown-item>Jannes Blobel</el-dropdown-item>
          <el-dropdown-item>Joschua Rothenbacher</el-dropdown-item>
          <el-dropdown-item>Fabienne Vatter</el-dropdown-item>
        </el-dropdown-menu>
      </template>
    </el-dropdown>

    <p>
      <el-input placeholder="name of your plant" style="left: 40px; width:60%; margin-right:40px; margin-bottom:20px" size="medium"></el-input>
      <el-button plain v-on:click="save()">Save!</el-button>
    </p>
  </div>
</template>

<script>
import pic1 from "../assets/plant1.png"
import pic2 from "../assets/plant2.png"
import pic3 from "../assets/plant3.png"
import pic4 from "../assets/plant4.png"
//import { ref } from 'vue'
export default {
  name: 'HelloWorld',
  props: {
    msg: String
  },
  el: "...",
  data: function() {
    return {
        radio1: 'Home',
        radio2: 'Add new',
        value: new Date(),
        checked: true,
        pics: [
          pic1,
          pic2,
          pic3,
          pic4
        ]
    };
  },
  methods: {
  }
}
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
div {
  text-align: center;
}
h3 {
  margin: 40px 0 0;
}
ul {
  list-style-type: none;
  padding: 0;
}
li {
  display: inline-block;
  margin: 0 10px;
}
a {
  color: #42b983;
}
.el-row {
  margin-bottom: 20px;
  &:last-child {
    margin-bottom: 0;
  }
}
.el-col {
  border-radius: 4px;
}
.bg-white {
  background: white;
}
.bg-green {
  background: #d3e6d9;
}
.grid1 {
  border-radius: 4px;
  min-height: 100px;
}
.grid2 {
  border-radius: 4px;
  min-height: 200px;
}
.grid3 {
  border-radius: 4px;
  min-height: 70px;
}
.row-bg {
  padding: 10px 0;
  background-color: #f9fafc;
}
.el-checkbox {
    color: #075735;
}
h1 {
  font-size: 40px;
  margin-top: 0px;
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
    margin-left: 20px;
}
button:hover {
    background-color: #0b380d;
}
button:active {
  background-color: #0b380d;
  box-shadow: 0 0px;
  transform: translateY(4px);
}
input {
  height: 25px;
  font-size: 18px;
}
select {
  height: 45px;
  font-size: 18px;
  color:#0b380d;
  box-shadow: 0 6px #82daa7;
  padding: 1px 30px 8px;
}
input.checkbox {
  font-size: 30px;
  float:middle;
  margin-right: .75em;
}
.el-radio-group {
  display: inline;
}
img {
  width: 200px;
}
.one {
  font-size: 20px;
}
</style>

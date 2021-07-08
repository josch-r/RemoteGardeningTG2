<template>
  <div class="hello">
    <h1>{{ msg }}</h1>

    <!-- plant overview -->
     <el-row :gutter="20">
      <el-col :span="5"><div class="grid">
        <h2>Gerthrude</h2>
        <router-link to="/overview/73231108155">
          <img src="../assets/plant1.png"/>
        </router-link>
        </div> 
      </el-col>
      <el-col :span="5"><div class="grid">
        <h2>Willi</h2>
        <img src="../assets/plant2.png"/>
        </div>
      </el-col>
      <el-col :span="5"><div class="grid">
        <h2>Tommy</h2>
        <img src="../assets/plant3.png"/>
        </div>
      </el-col>
      <el-col :span="5"><div class="grid">
        <h2>Bucky</h2>
        <img src="../assets/plant4.png"/>
        </div>
      </el-col>
    </el-row>
    <br><br>


    <!-- Add new plant -->
    <!-- <el-button-group>
      <router-link to="/add">
        <el-button style="margin-left: 10px;" id="button" icon="el-icon-plus">Add Plant!</el-button>
      </router-link>
    </el-button-group> -->

  </div>
</template>

<script>
export default {
  name: 'HelloWorld',
  props: {
    msg: String
  },
  el: "...",
  data: function() {
    return {
    };
  },
  methods: {
  }
}
</script>

<style scoped>
h2 {
  margin-top: 2px;
  margin-bottom: 2px;
}
.el-row {
  margin-bottom: 20px;
  margin-left: 10px;
}
.el-col {
  border-radius: 4px;
  margin-left: 10px;
}
.grid {
  background: white;
  border-radius: 4px;
  min-height: 200px;
}
h1 {
  font-size: 40px;
  margin-top: 0px;
  margin-left: 10px;
}
img {
  height: 200px;
  border-radius: 4px;
  min-height: 200px;
}
</style>

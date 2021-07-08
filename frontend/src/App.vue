<template>
  <HelloWorld msg="MY GARDEN"/>
  <router-view/>
</template>


<style lang="scss">
#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: left;
  color: #2c3e50;
}

#nav {
  margin-bottom: 30px;
  margin-top: 20px;
  a {
    font-weight: bold;
    color: #2c3e50;
    &.router-link-exact-active {
      color: #42b983;
    }
  }
}
#button {
    padding: 8px 20px;
    font-size: 17px;
    text-align: center;
    cursor: pointer;
    outline: none;
    color: #fff;
    background-color:#09800f;
    border: none;
    border-radius: 15px;
}
#button:hover {
    background-color: #0b380d;
    color: #fff;
}
#button:active {
  background-color: #0b380d;
  box-shadow: 0 0px;
  transform: translateY(4px); 
}
#button:focus {
  color:#09800f;
  background-color: #0b380d;
}
</style>

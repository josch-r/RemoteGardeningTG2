import { createRouter, createWebHistory } from 'vue-router'
import Home from '../views/Home.vue'
import Add from '../views/Add.vue'
import Overview from '../views/Overview.vue'

const routes = [
  {
    path: '/',
    name: 'Home',
    component: Home
  },
  {
    path: '/add/:id',
    name: 'Add',
    component: Add
  },
  {
    path: '/overview/:id',
    name: 'Overview',
    component: Overview
  },
]

const router = createRouter({
  history: createWebHistory(process.env.BASE_URL),
  routes
})

export default router

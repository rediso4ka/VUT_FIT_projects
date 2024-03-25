import React, { useContext } from 'react'
import classes from "./Admin.module.css"
import { Link, Outlet } from 'react-router-dom'
import Tabs, { TabsType } from '../../components/Tabs/Tabs'
import PageView from '../../components/PageView/PageView'
import { AppContext } from '../../context/AppContextProvider'
import { roles } from '../../utils/common'

const Admin = () => {
    const context = useContext(AppContext)

    const tabs: TabsType = [
        {
            to: "",
            text: "Events"
        },
        {
            to: "categories",
            text: "Categories"
        },
        {
            to: "places",
            text: "Places"
        },
        ...(context.user.role === roles.ADMIN ? [
            {
                to: "users",
                text: "Users"
            },
            {
                to: "logs",
                text: "Logs"
            }] : [])
    ]

    return (
        <PageView title="Admin panel">
            <Tabs prefix='/admin' tabs={tabs} />
            <Outlet />
        </PageView>
    )
}

export default Admin
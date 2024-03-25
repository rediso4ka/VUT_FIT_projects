import React from 'react'
import { Link, Outlet } from 'react-router-dom'
import PageView from '../../components/PageView/PageView'
import Tabs, { TabsType } from '../../components/Tabs/Tabs'

import classes from "./CreateEvent.module.css"

const tabs: TabsType = [
    {
        to: "",
        text: "Event"
    }
]

const CreateEvent = () => {
    return (
        <PageView scroll title='Create Event'>
            <Tabs prefix='/events/create' tabs={tabs} />
            <Outlet />
        </PageView>
    )
}

export default CreateEvent
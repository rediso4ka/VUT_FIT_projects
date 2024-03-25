import React, { useContext, useEffect, useState } from 'react'
import { Outlet, useParams } from 'react-router-dom'
import Tabs, { TabsType } from '../../components/Tabs/Tabs'
import PageView from '../../components/PageView/PageView'
import { AppContext } from '../../context/AppContextProvider'
import { roles } from '../../utils/common'

const EventView = () => {
  const { id } = useParams()

  const context = useContext(AppContext)

  const [authorId, setAuthorId] = useState<number | null>(null)

  const fetchAuthor = async () => {
    try {
      const response = await context.request!.get(`/event/${id}`)
      setAuthorId(response.data.authorId)
    } catch (error) {
      console.error(error)
    }
  }

  useEffect(
    () => {
      fetchAuthor()
    },
    []
  )

  const tabs: TabsType = [
    {
      to: "",
      text: "Event"
    },
    ...(
      (context.isAuth && context.user.id === authorId) ? [{
        to: "users",
        text: "Users"
      }] : []
    ),
    ...(
      (context.isAuth && context.user.role === roles.ADMIN) ? [{
        to: "logs",
        text: "Logs"
      }] : []
    )
  ]

  return (
    <PageView scroll title="Event details">
      <Tabs tabs={tabs} prefix={`/events/${id}`} />
      <Outlet />
    </PageView>
  )
}

export default EventView
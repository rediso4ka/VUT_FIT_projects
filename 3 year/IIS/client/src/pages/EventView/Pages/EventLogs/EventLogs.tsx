import React, { useContext, useEffect, useState } from 'react'
import TableView from '../../../../components/TableView/TableView'
import Table, { TableHeaderType } from '../../../../components/Table/Table'
import { AppContext, LoadingType, UserType, timeAgo } from '../../../../context/AppContextProvider'
import { SpringResponseType } from '../../../../utils/common'
import { useParams } from 'react-router-dom'

const dataKeys: TableHeaderType = {
  action: "Action",
  text: "Log",
  date: "Time",
}

type LogType = {
  admin: UserType
  date: string
  text: string
}

const EventLogs = () => {
  const {id} = useParams()
  const [logs, setLogs] = useState<LogType[]>([])

  const context = useContext(AppContext)

  const fetchCategories = async () => {
    context.setLoading(LoadingType.FETCHING)
    try {
      const response = await context.request!.get(`/event/${id}/logs`)

      const responses = await Promise.allSettled(
        response.data.logs.map(async (id: number) => await context.request!.get(`/event/log/${id}`))
      );

      const fulfilledResponses = responses
        .filter((r): r is PromiseFulfilledResult<SpringResponseType<LogType>> => r.status === "fulfilled")
        .map((r) => r.value)
        .filter((v) => v);

      setLogs(fulfilledResponses.map(({ data }) => data))
    } catch (error) {
      console.error(error)
    } finally {
      context.setLoading(LoadingType.NONE)
    }
  }

  useEffect(() => {
    fetchCategories()
  }, [])
  return (
    <TableView>
      <Table
        dataKeys={dataKeys}
        fmt={(key, value) => {
          if (key === "date") {
            return timeAgo.format(new Date(value))
          } 
          return value
        }}
        data={logs}
      />
    </TableView>
  )
}

export default EventLogs
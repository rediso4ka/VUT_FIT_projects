import React, { useContext, useEffect, useState } from 'react'
import TableView from '../../../../components/TableView/TableView'
import Table, { TableHeaderType } from '../../../../components/Table/Table'
import { AppContext, LoadingType, timeAgo } from '../../../../context/AppContextProvider'
import { SpringResponseType } from '../../../../utils/common'
import { UserType } from '../Users/Users'

const dataKeys: TableHeaderType = {
    admin: "User",
    text: "Log",
    date: "Time",
}

type LogType = {
    admin: UserType
    date: string
    text: string
}

const Logs = () => {

    const [logs, setLogs] = useState<LogType[]>([])
    const context = useContext(AppContext)

    const fetchCategories = async () => {
        context.setLoading(LoadingType.FETCHING)
        try {
            const response = await context.request!.get("/admin/logs")

            const responses = await Promise.allSettled(
                response.data.logs.map(async (id: number) => await context.request!.get(`/admin/log/${id}`))
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
                    } else if (key === "admin") {
                        return value.login
                    }
                    return value
                }}
                data={logs}
            />
        </TableView>
    )
}

export default Logs
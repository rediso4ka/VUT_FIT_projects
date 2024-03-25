import React, { useContext, useEffect, useState } from 'react'
import { AppContext, LoadingType } from '../../../../context/AppContextProvider'
import { EventType } from '../../../../utils/types'
import TableView from '../../../../components/TableView/TableView'
import Table, { TableHeaderType } from '../../../../components/Table/Table'
import RowActions from './components/RowActions/RowActions'
import { formatDate } from '../../../../utils/common'

const dataKeys: TableHeaderType = {
    id: "Id",
    title: "Title",
    dateFrom: "Start date",
    dateTo: "End date",
    placeId: "Place Id",
    authorId: "Author Id",
    categoryId: "Category Id",
    status: "Status",
}

const Events = () => {

    const context = useContext(AppContext)

    const [events, setEvents] = useState<EventType[]>([])

    const fetchEvents = async () => {
        context.setLoading(LoadingType.FETCHING)
        try {
            const response = await context.request!.get("/events")

            const responses = await Promise.allSettled(
                response.data.events.map(async (id: number) => await context.request!.get(`/event/${id}`))
            );

            const fulfilledResponses = responses
                .filter((r): r is PromiseFulfilledResult<any> => r.status === "fulfilled")
                .map((r) => r.value)
                .filter((v) => v);

            setEvents(fulfilledResponses.map(({ data }) => data))

        } catch (error) {
            console.error(error)
        } finally {
            context.setLoading(LoadingType.NONE)
        }
    }

    useEffect(() => {
        fetchEvents()
    }, [])

    return (
        <TableView>
            <Table
                dataKeys={dataKeys}
                data={events}
                fmt={(key, value) => {
                    if (key === "dateFrom" || key === "dateTo") {
                        return formatDate(value)
                    }
                    return value
                }}
                rowActions={(event) => (
                    <RowActions
                        events={events}
                        setEvents={setEvents}
                        event={event}
                    />
                )} />
        </TableView>
    )
}

export default Events
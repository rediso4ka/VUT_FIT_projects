import React, { useContext, useEffect, useState } from 'react'
import CreateTicketModal, { TicketType } from './modals/CreateTicketModal/CreateTicketModal'
import { AppContext } from '../../../../context/AppContextProvider'
import { SpringResponseType } from '../../../../utils/common'
import PageView from '../../../../components/PageView/PageView'
import Ticket from '../components/TicketInputs/components/Ticket/Ticket'

import classes from "./Tickets.module.css"

export type TicketTypeWithRegister = TicketType & { status: string, date: string }

const Tickets = () => {
    const context = useContext(AppContext)
    const [tickets, setTickets] = useState<TicketTypeWithRegister[]>([])

    const fetch = async () => {
        if (!context.user.id) return
        try {
            const response = await context.request!.get(`/user/${context.user.id}/tickets`)

            const ticketResponses = await Promise.allSettled(
                response.data.tickets.map(async (ticketId: number) => await context.request!.get(`/event/ticket/${ticketId}`))
            );

            const registersResponses = await Promise.allSettled(
                response.data.tickets.map(async (id: number) => await context.request!.get(`user/${context.user.id}/ticket/${id}`))
            );

            const registersFulfilledResponses = registersResponses
                .filter((r): r is PromiseFulfilledResult<SpringResponseType<{ ticketId: number, date: string, status: string }>> => r.status === "fulfilled")
                .map((r) => r.value)
                .filter((v) => v);

            const ticketFulfilledResponses = ticketResponses
                .filter((r): r is PromiseFulfilledResult<SpringResponseType<TicketType>> => r.status === "fulfilled")
                .map((r) => r.value)
                .filter((v) => v);

            setTickets(registersFulfilledResponses.map(({ data }) => {
                const { ticketId, date, status } = data

                const ticket = ticketFulfilledResponses.map(({ data }) => data).find(({ id }) => id! === ticketId!)
                return { ...ticket, date, status } as TicketTypeWithRegister
            }))
        } catch (error) {
            console.log(error)
        }
    }

    useEffect(() => {
        fetch()
    }, [context.user])

    return (
        <PageView scroll title='My tickets'>
            <div className={classes.container}>
                {tickets.map(ticket => <Ticket key={ticket.id} ticket={ticket} />)}
            </div>
        </PageView>
    )
}

export default Tickets
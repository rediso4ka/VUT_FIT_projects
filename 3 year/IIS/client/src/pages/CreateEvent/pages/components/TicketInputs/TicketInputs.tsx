import React from 'react'
import classes from "./TicketInputs.module.css"
import { TicketType } from '../../Tickets/modals/CreateTicketModal/CreateTicketModal'
import Ticket from './components/Ticket/Ticket'
import NewTicket from './components/NewTicket/NewTicket'
import { TicketTypeWithRegister } from '../../Tickets/Tickets'
import uuid from 'react-uuid'
import { EventType } from '../../../../../utils/types'

type PropsType = {
    tickets: TicketTypeWithRegister[],
    createTicket: (inputs: TicketTypeWithRegister) => void
    deleteTicket: (i: number) => () => void
    updateTicket: (i: number) => (input: TicketTypeWithRegister) => void
    enableNewTicket?: boolean,
    event?: EventType
    getTicket?: (ticket: TicketType ) => Promise<any>
}

const TicketInputs = ({
    tickets,
    createTicket,
    deleteTicket,
    updateTicket,
    enableNewTicket,
    event,
    getTicket
}: PropsType) => {
    
    return (
        <div className={classes.container}>
            {tickets.map((ticket, i) => (
                <Ticket
                    key={uuid()}
                    event={event}
                    ticket={ticket}
                    deleteTicket={deleteTicket(ticket.id!)}
                    updateTicket={updateTicket(ticket.id!)}
                    getTicket={getTicket}
                />
            ))}
            {enableNewTicket && <NewTicket createTicket={createTicket} />}
        </div>
    )
}

export default TicketInputs
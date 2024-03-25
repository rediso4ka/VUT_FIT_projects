import React, { useState } from 'react'
import classes from "./NewTicket.module.css"
import CreateTicketModal, { TicketType } from '../../../../Tickets/modals/CreateTicketModal/CreateTicketModal'
import { TicketTypeWithRegister } from '../../../../Tickets/Tickets'
import { Icon } from '@iconify/react'
import icons from '../../../../../../../utils/icons'

type PropsType = {
    createTicket: (inputs: TicketTypeWithRegister) => void
}

const NewTicket = ({
    createTicket
}: PropsType) => {
    const [isActive, setIsActive] = useState(false)

    const onSubmit = (inputs: TicketTypeWithRegister) => {
        createTicket(inputs)
        onClose()
    }

    const onClose = () => {
        setIsActive(false)
    }
  return (
      <>
          <button className={classes.empty} onClick={() => setIsActive(true)}>
              <span className={classes.border}>
                  <Icon icon={icons.add} width={60} height={60} />
              </span>
          </button>
          {isActive && (
              <CreateTicketModal
                title='Create new ticket'
                textProceed='Save'
                icon={icons.plus}
                onSubmit={onSubmit}
                onClose={onClose}
              />
          )}
      </>
  )
}

export default NewTicket
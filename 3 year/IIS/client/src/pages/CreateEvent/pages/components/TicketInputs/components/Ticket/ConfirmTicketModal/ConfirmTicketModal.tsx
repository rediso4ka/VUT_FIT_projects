import React from 'react'
import Modal from '../../../../../../../../components/Modal/Modal'
import icons from '../../../../../../../../utils/icons'

type PropsType = {
    onClose: () => void
    onSubmit: () => void
    ticketName: string
    eventName: string
}

/**
 * 
 * @param param0 
 * @returns 
 */
const ConfirmTicketModal = ({
    onClose,
    onSubmit,
    ticketName,
    eventName
}: PropsType) => {
  return (
      <Modal title="Confirm to get a ticket"
          textProceed="Confirm"
          textCancel="Cancel"
          onClose={onClose}
          onSubmit={onSubmit}
          icon={icons.danger}>
          Do you want to get a ticket "{ticketName}"?
      </Modal>
  )
}

export default ConfirmTicketModal
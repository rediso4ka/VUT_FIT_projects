import React from 'react'
import Modal from '../../../../components/Modal/Modal'
import icons from '../../../../utils/icons'

type PropsType = {
    onClose: () => void
    onSubmit: () => void
}

const EventDeleteModal = ({
    onClose,
    onSubmit
}: PropsType) => {
  return (
      <Modal title="Delete event?"
          textProceed="Confirm"
          textCancel="Cancel"
          onClose={onClose}
          onSubmit={onSubmit}
          icon={icons.trash}>
          Confirm this action
      </Modal>
  )
}

export default EventDeleteModal
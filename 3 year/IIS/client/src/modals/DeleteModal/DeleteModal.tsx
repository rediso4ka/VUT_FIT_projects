import React from 'react'
import Modal from '../../components/Modal/Modal'
import icons from '../../utils/icons'

type PropsType = {
    onClose: () => void
    onSubmit: () => void
    title: string
}

const DeleteModal = ({
    onClose,
    onSubmit,
    title
}: PropsType) => {
  return (
      <Modal
          title={title}
          textProceed="Delete"
          textCancel="Cancel"
          onClose={onClose}
          onSubmit={onSubmit}
          icon={icons.trash}>
          Please, confirm this action
      </Modal>
  )
}

export default DeleteModal
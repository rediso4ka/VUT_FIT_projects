import React, { useState } from 'react'
import classes from "./CreatePlaceModal.module.css"
import Modal, { ModalStyles } from '../../../../../../components/Modal/Modal'
import { PlaceType } from '../../Places'
import Input from '../../../../../../components/Input/Input'
import icons from '../../../../../../utils/icons'
import { CategoryType } from '../../../Categories/Categories'
import Textarea from '../../../../../../components/Textarea/Textarea'
import { status } from '../../../../../../utils/common'

const initialInputs: PlaceType = {
    "id": -1,
    "name": "",
    "description": "",
    "address": "",
    "status": status.ACCEPTED
}

type PropsType = {
    inputs?: PlaceType
    places: PlaceType[]
    onSubmit: (inputs: PlaceType) => void
    onClose: () => void
    title: string,
    textProceed: string
    icon: icons
}

const CreatePlaceModal = ({
    inputs: defaultInputs,
    places,
    onSubmit,
    onClose,
    title,
    textProceed,
    icon
}: PropsType) => {
    const [inputs, setInputs] = useState(defaultInputs ?? initialInputs)

    const onChange = (e: React.ChangeEvent<HTMLInputElement | HTMLTextAreaElement>) => {
        setInputs(prev => ({ ...prev, [e.target.name]: e.target.value }))
    }

    const __onSubmit = () => {
        onSubmit(inputs)
    }
    
  return (
      <Modal
          title={title}
          textProceed={textProceed}
          textCancel={'Cancel'}
          onClose={onClose}
          onSubmit={__onSubmit}
          icon={icon}
          type={ModalStyles.Inputs}
          disabled={inputs.name.length === 0 || inputs.address.length === 0}>
          <Input required label='Name' name='name' value={inputs.name} onChange={onChange} />
          <Input required label='Address' name='address' value={inputs.address} onChange={onChange} />
          <Textarea label='Description' name='description' value={inputs.description} onChange={onChange} />
      </Modal>
  )
}

export default CreatePlaceModal
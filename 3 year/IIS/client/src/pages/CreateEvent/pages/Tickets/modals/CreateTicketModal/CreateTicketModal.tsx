import React, { useState } from 'react'
import Modal, { ModalStyles } from '../../../../../../components/Modal/Modal'
import icons from '../../../../../../utils/icons'
import Input from '../../../../../../components/Input/Input'
import Textarea from '../../../../../../components/Textarea/Textarea'
import { TicketTypeWithRegister } from '../../Tickets'

type PropsType = {
    title: string,
    icon: icons,
    textProceed: string,
    defaultInputs?: TicketTypeWithRegister
    onClose: () => void,
    onSubmit: (inputs: TicketTypeWithRegister) => void,
}

export type TicketType = {
    id: number | null
    name: string,
    price: string,
    capacity: string,
    description: string
    eventId: number | null
}

const initialInputs: TicketTypeWithRegister = {
    id: null,
    name: "",
    price: "",
    capacity: "",
    description: "",
    date: "",
    status: "",
    eventId: null
}

const CreateTicketModal = ({
    title,
    textProceed,
    icon,
    defaultInputs,
    onClose,
    onSubmit,
}: PropsType) => {
    const [inputs, setInputs] = useState(defaultInputs ?? initialInputs)

    const onChange = (e: React.ChangeEvent<HTMLInputElement | HTMLTextAreaElement>) => {
        setInputs(prev => ({ ...prev, [e.target.name]: e.target.value }))
    }

    const __onSubmit = () => {
        onSubmit(inputs)
    }

    const isDisabled = (
        inputs.name.length === 0
    )

    return (
        <Modal
            title={title}
            textProceed={textProceed}
            textCancel="Cancel"
            onClose={onClose}
            onSubmit={__onSubmit}
            icon={icon}
            disabled={isDisabled}
        type={ModalStyles.Inputs}>
            <Input required label='Name' name='name' value={inputs.name} onChange={onChange} />
            <Input label='Price' name='price' value={inputs.price} min={0} type='number' onChange={onChange} />
            <Input label='Capacity' name='capacity' value={inputs.capacity} min={1} type='number' onChange={onChange} />
            <Textarea label='Description' name='description' value={inputs.description} onChange={onChange} />
        </Modal>
    )
}

export default CreateTicketModal
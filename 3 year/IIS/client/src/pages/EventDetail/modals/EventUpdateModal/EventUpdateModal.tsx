import React, { useState } from 'react'
import classes from "./EventUpdateModal.module.css"
import Modal, { ModalStyles } from '../../../../components/Modal/Modal'
import icons from '../../../../utils/icons'
import { EventType } from '../../../../utils/types'
import DateInput, { DateChangeType } from '../../../../components/DateInput/DateInput'
import Input from '../../../../components/Input/Input'
import Textarea from '../../../../components/Textarea/Textarea'

type PropsType = {
    event: EventType,
    onClose: () => void,
    onSubmit: (event: EventType) => void
}

const EventUpdateModal = ({
    event,
    onClose,
    onSubmit
}: PropsType) => {

    const [inputs, setInputs] = useState(event)

    const onChange = (e: React.ChangeEvent<HTMLInputElement | HTMLTextAreaElement>) => {
        setInputs(prev => ({ ...prev, [e.target.name]: e.target.value }))
    }

    const onDateChange: DateChangeType = (name, value) => {
        setInputs(prev => ({ ...prev, [name]: value?.toISOString() }))
    }

    const __onSubmit = () => {
        onSubmit(inputs)
    }

    return (

        <Modal title="Update event"
            textProceed="Save"
            textCancel="Cancel"
            onClose={onClose}
            onSubmit={__onSubmit}
            icon={icons.pen}
            type={ModalStyles.Inputs}
            disabled={
                inputs.dateFrom.length === 0 ||
                inputs.title.length === 0
            }
            >
            <Input required label='Title' name='title' value={inputs.title} onChange={onChange} />
            <DateInput required label='Start date' name='dateFrom' value={inputs.dateFrom} onChange={onDateChange} />
            <DateInput label='End date' name='dateTo' value={inputs.dateTo} onChange={onDateChange} />
            <Textarea label='Description' name='description' value={inputs.description} onChange={onChange} />
        </Modal>
    )
}

export default EventUpdateModal
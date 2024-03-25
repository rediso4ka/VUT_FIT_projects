import React, { useState } from 'react'
import Modal, { ModalStyles } from '../../../../../../components/Modal/Modal'
import Input from '../../../../../../components/Input/Input'
import InputLabel from '../../../../../../components/InputLabel/InputLabel'
import Dropdown from '../../../../../../components/Dropdown/Dropdown'
import icons from '../../../../../../utils/icons'
import { CategoryType, categoriesToDropdown } from '../../Categories'
import classes from './CreateCategoryModal.module.css'
import { status } from '../../../../../../utils/common'

export type CategoryInput = {
    name: string,
    status: string,
    parentCategory: number | null
}

const initialInputs: CategoryInput = {
    name: "",
    status: status.ACCEPTED,
    parentCategory: null
}

type PropsType = {
    inputs?: CategoryInput
    categories: CategoryType[]
    onSubmit: (inputs: CategoryInput) => void
    onClose: () => void
    title: string,
    textProceed: string
    icon: icons
}

const CreateCategoryModal = ({
    inputs: defaultInputs,
    categories,
    onSubmit,
    onClose,
    title,
    textProceed,
    icon
}: PropsType) => {

    const [inputs, setInputs] = useState<CategoryInput>(defaultInputs ?? initialInputs)

    const onChange = (e: React.ChangeEvent<HTMLInputElement>) => {
        setInputs(prev => ({ ...prev, [e.target.name]: e.target.value }))
    }

    const onDropdownChange = (value: string | number, name: string) => {
        setInputs(prev => ({ ...prev, parentCategory: value === null ? value : +value }))
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
            disabled={inputs.name.length === 0}>
            <Input required label='Name' name='name' value={inputs.name} onChange={onChange} />
            <InputLabel value='Parent category'>
                <Dropdown value={String(inputs.parentCategory)} items={categoriesToDropdown(categories)} onChange={onDropdownChange} />
            </InputLabel>
        </Modal>
    )
}

export default CreateCategoryModal
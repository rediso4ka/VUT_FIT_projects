import React, { useState } from 'react'
import Modal, { ModalStyles } from '../../../../../../components/Modal/Modal'
import Input from '../../../../../../components/Input/Input'
import icons from '../../../../../../utils/icons'
import { UserType } from '../../../../../../context/AppContextProvider'

type PropsType = {
    inputs: UserType,
    onClose: () => void,
    onSubmit: (inputs: UserType) => void
}

const UserSettingsModal = ({
    inputs: defaultInputs,
    onClose,
    onSubmit
}: PropsType) => {
    const [inputs, setInputs] = useState(defaultInputs)

    const __onSubmit = () => {
        onSubmit(inputs)
    }

    const onChange = (e: React.ChangeEvent<HTMLInputElement>) => {
        setInputs(prev => ({ ...prev, [e.target.name]: e.target.value }))
    }

    return (
        <Modal
            title={"Update user"}
            textProceed={"Update"}
            textCancel={'Cancel'}
            onClose={onClose}
            onSubmit={__onSubmit}
            icon={icons.settings}
            type={ModalStyles.Inputs}
            disabled={(
                inputs.firstname === null ||
                inputs.lastname === null ||
                inputs.firstname?.length === 0 ||
                inputs.lastname?.length === 0
        )}>
            <Input required label='Name' name='firstname' value={inputs.firstname ?? ""} onChange={onChange} />
            <Input required label='Surname' name='lastname' value={inputs.lastname ?? ""} onChange={onChange} />
            <Input label='Phone' name='phone' value={inputs.phone ?? ""} onChange={onChange} />
            
        </Modal>
    )
}

export default UserSettingsModal
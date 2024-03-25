import React, { useContext, useState } from 'react'
import { AppContext } from '../../context/AppContextProvider'
import { Link } from 'react-router-dom'

import classes from "./Register.module.css"
import { roles } from '../../utils/common'
import Input from '../../components/Input/Input'
import Button from '../../components/Button/Button'
import Tabs, { TabsType } from '../../components/Tabs/Tabs'

type PropsType = {
    role: roles
}

const initialInputs = {
    "login": "",
    "password": "",
    "email": "",
    // "firstname": "Alex",
    // "lastname": "Turytsia",
    // "phone": "12345689987",
    // "email": "xturyt00@fit.vut.cz",
    // "roles": "ROLE_USER"
}

const tabs: TabsType = [
    {
        to: "/register/user",
        text: "User"
    },
    {
        to: "/register/manager",
        text: "Manager"
    },
    {
        to: "/register/admin",
        text: "Admin"
    }
]

const Register = ({
    role
}: PropsType) => {
    const context = useContext(AppContext)

    const [inputs, setInputs] = useState(initialInputs)

    const onChange = (e: React.ChangeEvent<HTMLInputElement>) => {
        setInputs(prev => ({ ...prev, [e.target.name]: e.target.value }))
    }

    const onSubmit = () => {
        context.register(inputs.login, inputs.email, inputs.password, role)
    }

    return (
        <div className={classes.outer}>
            <div className={classes.container}>
                <Tabs tabs={tabs} prefix={''} />
                <div className={classes.inner}>
                    <div className={classes.title}>
                        <h4>Register</h4>
                    </div>
                    <Input value={inputs.login} name="login" onChange={onChange} placeholder='Login' type='text' />
                    <Input value={inputs.email} name="email" onChange={onChange} placeholder='Email' type='email' />
                    <Input value={inputs.password} name="password" onChange={onChange} placeholder='Password' type='password' />
                    <div className={classes.actions}>
                        <Button style='invert' onClick={onSubmit}>Register</Button>
                        <Link to="/login">Already have an account?</Link>
                    </div>
                </div>
            </div>
        </div>
    )
}

export default Register
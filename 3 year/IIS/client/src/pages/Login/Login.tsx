import React, { useContext, useState } from 'react'
import { AppContext } from '../../context/AppContextProvider'
import { Link } from 'react-router-dom'

import classes from "./Login.module.css"
import Button from '../../components/Button/Button'
import Input from '../../components/Input/Input'

const initialInputs = {
    "login": "",
    "password": "",
}

const Login = () => {
    const context = useContext(AppContext)

    const [inputs, setInputs] = useState(initialInputs)

    const onChange = (e: React.ChangeEvent<HTMLInputElement>) => {
        setInputs(prev => ({ ...prev, [e.target.name]: e.target.value }))
    }

    const onSubmit = () => {
        context.login(inputs.login, inputs.password)
    }

    return (
        <div className={classes.container}>
            <div className={classes.title}>
                <h4>Login</h4>
            </div>
            <Input value={inputs.login} name="login" onChange={onChange} placeholder='Login' type='text' />
            <Input value={inputs.password} name="password" onChange={onChange} placeholder='Password' type='password' />
            <div className={classes.actions}>
                <Button style='invert' onClick={onSubmit}>Login</Button>
                <Link to="/register/user">I don't have an account</Link>
            </div>
        </div>
    )
}

export default Login
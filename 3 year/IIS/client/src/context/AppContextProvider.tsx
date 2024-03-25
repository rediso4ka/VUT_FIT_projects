import axios, { AxiosInstance } from 'axios'
import React, { createContext, useEffect, useMemo, useState } from 'react'
import { roles } from '../utils/common'
import Loading from '../components/Loading/Loading'
import { useNavigate } from 'react-router-dom'
import { ToastContainer, toast } from 'react-toastify';

import TimeAgo from 'javascript-time-ago'

// English.
import en from 'javascript-time-ago/locale/en'

TimeAgo.addDefaultLocale(en)

export const timeAgo = new TimeAgo('en-US')

type PropsType = {
  children: React.ReactNode
}

export type UserType = {
  id: number | null,
  login: string | null,
  firstname: string | null,
  lastname: string | null,
  phone: string | null,
  email: string | null,
  role: "ROLE_USER" | "ROLE_ADMIN" | "ROLE_MANAGER" | null
}

type AppContextType = {
  request: AxiosInstance | null,
  isAuth: boolean,
  register: (login: string, email: string, password: string, role: roles) => void,
  login: (login: string, password: string) => void,
  logout: () => void,
  user: UserType
  setLoading: React.Dispatch<React.SetStateAction<LoadingType>>
}

const initialUser: UserType = {
  "id": null,
  "login": null,
  "firstname": null,
  "lastname": null,
  "phone": null,
  "email": null,
  "role": null
}

const context: AppContextType = {
  request: null,
  isAuth: false,
  register: () => { },
  login: () => { },
  logout: () => { },
  user: initialUser,
  setLoading: () => {}
}

export enum LoadingType {
  FETCHING,
  LOADING,
  NONE
}

export const AppContext = createContext(context)

export const floatingRoot = document.getElementById("portal")

/**
 * 
 * @param param0 
 * @returns 
 */
const AppContextProvider = ({ children }: PropsType) => {

  const localUser = localStorage.getItem("user")

  const navigate = useNavigate()

  const [loading, setLoading] = useState<LoadingType>(LoadingType.NONE)
  const [token, setToken] = useState(localStorage.getItem("token"))
  const [user, setUser] = useState<UserType>(localUser ? JSON.parse(localUser) : initialUser)

  // http://localhost:8080
  // https://actions-and-events.azurewebsites.net

  const request = useMemo(
    () => axios.create({
      baseURL: 'http://localhost:8080',
      headers: token ? {
        "Authorization": `Bearer ${token}`
      } : {}
    }),
    [token]
  )


  const getUser = async () => {
    // if (localUser) {
    //   setUser(JSON.parse(localUser))
    //   return
    // }
    
    try {
      const response = await request.get<typeof initialUser>("/user")
      setUser(response.data)
      localStorage.setItem("user", JSON.stringify(response.data))
    } catch (error) {
      logout()
    }
  }

  const register = async (login: string, email: string, password: string, role: roles) => {
    setLoading(LoadingType.LOADING)
    try {
      const response = await request.post<typeof initialUser>("/auth/register", {
        ...initialUser,
        login,
        password,
        email,
        roles: role
      })
      if (response.status === 200) {
        navigate("/login")
      }
      // setUser(response.data)
      // localStorage.setItem("user", JSON.stringify(response.data))
    } catch (error) {
      setUser(initialUser)
    } finally {
      setLoading(LoadingType.NONE)
    }
  }

  const login = async (login: string, password: string) => {
    setLoading(LoadingType.LOADING)
    try {
      const response = await request.post<{ token: string }>("/auth/login", {
        login,
        password
      })
      setToken(response.data.token)
      localStorage.setItem("token", response.data.token)
      localStorage.setItem("user", JSON.stringify(response.data.token))
    } catch (error) {
      logout()
    } finally {
      setLoading(LoadingType.NONE)
    }
  }

  const logout = () => {
    setToken(null)
    setUser(initialUser)
    localStorage.setItem("user", "")
    localStorage.removeItem("token")
  }

  const value: AppContextType = {
    request,
    isAuth: !!token,
    register,
    login,
    logout,
    user,
    setLoading
  }

  useEffect(() => {
    const displayMessage = (data: any) => {
      const message = data?.message?.message ? data?.message?.message : data?.message
      const status: "error" | "success" = data?.message?.status ? data?.message?.status : data?.status
      toast[status](message, {
        position: "top-right",
        autoClose: 5000,
        hideProgressBar: false,
        closeOnClick: true,
        pauseOnHover: true,
        draggable: true,
        progress: undefined,
        theme: "light",
        style: {
          top: 58
        }
      });
    }
    request.interceptors.response.use(response => {
      if (response?.data?.message) {
        displayMessage(response?.data)
      }
      return response
    }, error => {
      // console.log(error)
      if (error?.response?.data?.message) {
        displayMessage(error?.response?.data)
      }
      if (error.response.data.message === 'Invalid token') {
        console.log("Token expired")
        logout()
        navigate('/login')
      }
    })
  }, [request])

  useEffect(() => {

    getUser()
  }, [token])


  return (
    <AppContext.Provider value={value}>
      <ToastContainer />
      {[LoadingType.LOADING, LoadingType.FETCHING].includes(loading) && <Loading />}
      {children}
    </AppContext.Provider>
  )
}

export default AppContextProvider
import React, { useContext, useEffect, useState } from 'react'
import TableView from '../../../../components/TableView/TableView'
import Table from '../../../../components/Table/Table'
import { AppContext, LoadingType } from '../../../../context/AppContextProvider'
import { SpringResponseType } from '../../../../utils/common'
import RowActions from './components/RowActions/RowActions'

export type UserType = {
  "id": number,
  "email": string,
  "login": string,
  "name": string | null,
  "surname": string | null,
  "phone": string | null,
  "role": "ROLE_USER" | "ROLE_MANAGER" | "ROLE_ADMIN"
}

const dataKeys = {
  "id": "Id",
  "email": "E-Mail",
  "login": "Login",
  "name": "Firstname",
  "surname": "Lastname",
  "phone": "Phone",
  "role": "Role"
}

const Users = () => {
  const context = useContext(AppContext)

  const [users, setUsers] = useState<UserType[]>([])

  const fetchCategories = async () => {
    context.setLoading(LoadingType.FETCHING)
    try {
      const response = await context.request!.get("/users")

      const responses = await Promise.allSettled(
        response.data.users.map(async (id: number) => await context.request!.get(`/user/${id}`))
      );

      const fulfilledResponses = responses
        .filter((r): r is PromiseFulfilledResult<SpringResponseType<any>> => r.status === "fulfilled")
        .map((r) => r.value)
        .filter((v) => v);
      setUsers(fulfilledResponses.map(({ data }) => data))
    } catch (error) {
      console.error(error)
    } finally {
      context.setLoading(LoadingType.NONE)
    }
  }

  useEffect(() => {
    fetchCategories()
  }, [])


  return (
    <TableView>
      <Table
        dataKeys={dataKeys}
        data={users}
        rowActions={(user) => (
          <RowActions
            user={user}
            setUsers={setUsers}
            users={users}
          />
        )}
      />
    </TableView>
  )
}

export default Users
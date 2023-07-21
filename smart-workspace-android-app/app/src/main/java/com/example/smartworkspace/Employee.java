package com.example.smartworkspace;

class Employee {

    private String UserName;
    private String EmpID;
    private String RFID;
    private String Password;
    private String Phone;
    private String Light;
    private String Fan;

    Employee(String userName, String empID, String rfid, String password, String phone, String light, String fan){
        setEmpID(empID);
        setFan(fan);
        setLight(light);
        setPassword(password);
        setUserName(userName);
        setRFID(rfid);
        setPhone(phone);
    }

    public String getUserName() {
        return UserName;
    }

    public void setUserName(String userName) {
        UserName = userName;
    }

    public String getEmpID() {
        return EmpID;
    }

    public void setEmpID(String empID) {
        EmpID = empID;
    }

    public String getRFID() {
        return RFID;
    }

    public void setRFID(String RFID) {
        this.RFID = RFID;
    }

    public String getPassword() {
        return Password;
    }

    public void setPassword(String password) {
        Password = password;
    }

    public String getPhone() {
        return Phone;
    }

    public void setPhone(String phone) {
        Phone = phone;
    }

    public String getLight() {
        return Light;
    }

    public void setLight(String light) {
        Light = light;
    }

    public String getFan() {
        return Fan;
    }

    public void setFan(String fan) {
        Fan = fan;
    }
}

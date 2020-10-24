import React, { Component } from 'react'

class LoginForm extends Component {
    constructor(props) {
        super(props)
        this.state = {
            remember: false,
        }

        this.formLogin = React.createRef()
        this.formPass = React.createRef()
    }

    render() {
        return (
            <div className="login-page-form">
                <h1 className="hug">
                    Welcome to <span>WIS FIT</span>
                </h1>

                <p className="subtitle">
                    Access to FIT information system is allowed only to
                    authorized users.
                </p>

                <form>
                    <div
                        className="loginInputGroup"
                        onClick={() => this.formLogin.current.focus()}
                    >
                        <div className="loginInputGroup__label">Login</div>
                        <input
                            autoFocus
                            ref={this.formLogin}
                            type="text"
                            className="loginInputGroup__input"
                        />
                    </div>

                    <div
                        className="loginInputGroup loginInputGroup--last"
                        onClick={() => this.formPass.current.focus()}
                    >
                        <div className="loginInputGroup__label">Password</div>
                        <input
                            ref={this.formPass}
                            type="password"
                            className="loginInputGroup__input"
                        />
                    </div>

                    <div className="form-row">
                        <label className="remember-me">
                            Remember me
                            <input
                                type="checkbox"
                                className="checkbox"
                                value={this.state.remember}
                                onClick={() => {
                                    this.setState({
                                        remember: !this.state.remember,
                                    })
                                }}
                                id="remember-me-checkbox"
                            />
                            <span className="checkmark" />
                        </label>

                        <span className="forgot-password">Forgot password</span>
                    </div>

                    <div className="form-row">
                        <input
                            className="btn btn--primary"
                            type="submit"
                            value="Login"
                            onClick={() => {
                                this.props.onSubmitClick()
                            }}
                        />
                    </div>

                    <p className="loginCopy">
                        © Faculty of information technology, BUT, Božetěchova 2,
                        612 66 Brno
                        <br />
                        For questions or comments contact{' '}
                        <a href="mailto:lampa@fit.vutbr.cz">
                            lampa@fit.vutbr.cz
                        </a>
                    </p>
                </form>
            </div>
        )
    }
}

export default LoginForm

## Description of the Algorithm

Notation: ![equation](http://latex.codecogs.com/gif.latex?\hat{q}) denotes the quaternion estimate, ![equation](http://latex.codecogs.com/gif.latex?P) denotes the error covariance matrix. The superscript ![equation](http://latex.codecogs.com/gif.latex?-) denotes the 'prior' version of a variable. ![equation](http://latex.codecogs.com/gif.latex?Q) and ![equation](http://latex.codecogs.com/gif.latex?R) denote the system input and output covariance matrices respectively.

Initialise: ![equation](http://latex.codecogs.com/gif.latex?\hat{q},%20P,%20Q,%20R)

At each sampling instant with sample time ![equation](http://latex.codecogs.com/gif.latex?\Delta%20t) and ![equation](http://latex.codecogs.com/gif.latex?n) reference vectors ![equation](http://latex.codecogs.com/gif.latex?\mathbf{r}_{1},\dots,\mathbf{r}_{n}):

### 1. Prediction step: with angular velocity measurement ![equation](http://latex.codecogs.com/gif.latex?\omega)

#### 1.1. Propagate quaternion

![equation](http://latex.codecogs.com/gif.latex?\hat{q}^{-}%20\gets%20\hat{q}+\dfrac{1}{2}\Xi\left(\hat{q}\right)\omega\Delta%20t)

where

![equation](http://latex.codecogs.com/gif.latex?\Xi\left(q\right)%20:=%20\begin{bmatrix}%20q_{4}%20&%20-q_{3}%20&%20q_{2}%20\\\\%20q_{3}%20&%20q_{4}%20&%20-q_{1}%20\\\\%20-q_{2}%20&%20q_{1}%20&%20q_{4}%20\\\\%20-q_{1}%20&%20-q_{2}%20&%20-q_{3}%20\end{bmatrix})

#### 1.2. Propagate error covariance

![equation](http://latex.codecogs.com/gif.latex?P^{-}%20\gets%20P%20+%20\left(F\left(\omega\right)P%20+%20PF\left(\omega\right)^{\top}%20+%20Q\right)\Delta%20t)

where

![equation](http://latex.codecogs.com/gif.latex?F\left(\omega\right)%20:=%20\begin{bmatrix}%200%20&%20-\omega_{3}%20&%20\omega_{2}%20\\\\%20\omega_{3}%20&%200%20&%20-\omega_{1}%20\\\\%20-\omega_{2}%20&%20\omega_{1}%20&%200\end{bmatrix})

### 2. Update step: with attitude sensor measurement vector ![equation](http://latex.codecogs.com/gif.latex?y)

#### 2.1. Compute output Jacobian

![equation](http://latex.codecogs.com/gif.latex?H%20\gets%20\begin{bmatrix}%20\operatorname{skew}\left(\operatorname{Rot}\left(\hat{q}^{-}\right)^{\top}\mathbf{r}_{1}\right)%20\\\\%20\vdots%20\\\\%20\operatorname{skew}\left(\operatorname{Rot}\left(\hat{q}^{-}\right)^{\top}\mathbf{r}_{n}\right)\end{bmatrix})

where

![equation](http://latex.codecogs.com/gif.latex?\operatorname{skew}\left(x\right)%20:=%20\begin{bmatrix}%200%20&%20-x_{3}%20&%20x_{2}%20\\\\%20x_{3}%20&%200%20&%20-x_{1}%20\\\\%20-x_{2}%20&%20x_{1}%20&%200\end{bmatrix})

![equation](http://latex.codecogs.com/gif.latex?\operatorname{Rot}\left(q\right)%20:=%20\begin{bmatrix}%20q_{1}^{2}%20-%20q_{2}^{2}%20-%20q_{3}^{2}%20+%20q_{4}^{2}%20&%202\left(q_{1}q_{2}%20-%20q_{3}q_{4}\right)%20&%202\left(q_{1}q_{3}%20+%20q_{2}q_{4}\right)%20\\\\%202\left(q_{1}q_{2}%20+%20q_{3}q_{4}\right)%20&%20-q_{1}^{2}%20+%20q_{2}^{2}%20-%20q_{3}^{2}%20+%20q_{4}^{2}%20&%202\left(q_{2}q_{3}%20-%20q_{1}q_{4}\right)%20\\\\%202\left(q_{1}q_{3}%20-%20q_{2}q_{4}\right)%20&%202\left(q_{2}q_{3}%20+%20q_{1}q_{4}\right)%20&%20-q_{1}^{2}%20-%20q_{2}^{2}%20+%20q_{3}^{2}%20+%20q_{4}^{2}%20\end{bmatrix})

#### 2.2. Compute Kalman gain

![equation](http://latex.codecogs.com/gif.latex?K%20\gets%20P^{-}H^{\top}\left(HP^{-}H^{\top}%20+%20R\right)^{-1})

#### 2.3. Local state update

![equation](http://latex.codecogs.com/gif.latex?\delta%20\mathbf{x}%20\gets%20K\left(y%20-%20h\left(\hat{q^{-}}\right)\right))

where

![equation](http://latex.codecogs.com/gif.latex?h\left(q\right)%20:=%20\begin{bmatrix}%20\operatorname{Rot}\left(q\right)^{\top}\mathbf{r}_{1}%20\\\\%20\vdots%20\\\\%20\operatorname{Rot}\left(q\right)^{\top}\mathbf{r}_{n}%20\end{bmatrix})

#### 2.4. Quaternion update

![equation](http://latex.codecogs.com/gif.latex?\hat{q}%20\gets%20\hat{q}^{-}%20+%20\dfrac{1}{2}\Xi\left(\hat{q}^{-}\right)%20\delta\mathbf{x})

#### 2.5. Normalisation

![equation](http://latex.codecogs.com/gif.latex?\hat{q}%20\gets%20\dfrac{\hat{q}}{\left\Vert%20\hat{q}\right\Vert})

#### 2.6. Error covariance update

![equation](http://latex.codecogs.com/gif.latex?P%20\gets%20\left(I%20-%20KH\right)P^{-})

## Efficiently computing the Kalman Gain

In general, if given the linear system ![equation](http://latex.codecogs.com/gif.latex?Ax=b), it is faster to obtain ![equation](http://latex.codecogs.com/gif.latex?x) by solving the linear system than by computing ![equation](http://latex.codecogs.com/gif.latex?x=A^{-1}b) (as the matrix inversion followed by multiplication can be expensive). We exploit this to efficiently find the Kalman gain without computing any matrix inverses. First rearrange the Kalman gain equation to

![equation](http://latex.codecogs.com/gif.latex?\left(HP^{-}H^{\top}%20+%20R\right)K^{\top}%20=%20HP^{-})

Since ![equation](http://latex.codecogs.com/gif.latex?HP^{-}H^{\top}%20+%20R) is a positive definite matrix, we can use the Cholesky decomposition to solve for the columns of ![equation](http://latex.codecogs.com/gif.latex?K^{\top}). The Cholesky decomposition involves decomposing symmetric definite ![equation](http://latex.codecogs.com/gif.latex?A) into ![equation](http://latex.codecogs.com/gif.latex?LL^{\top}), where ![equation](http://latex.codecogs.com/gif.latex?L) is a lower triangular matrix. We then solve ![equation](http://latex.codecogs.com/gif.latex?Lz=b) for ![equation](http://latex.codecogs.com/gif.latex?z), then followed by ![equation](http://latex.codecogs.com/gif.latex?L^{\top}x=z). To see how this works, consider

![equation](http://latex.codecogs.com/gif.latex?\begin{align}%20Ax%20&=%20b%20\\\\%20LL^{\top}x%20&=%20b%20\\\\%20L^{\top}x%20&=%20L^{-1}b%20\\\\%20L^{\top}x%20&=%20z%20\end{align})

Hence we will require solving three linear systems (since ![equation](http://latex.codecogs.com/gif.latex?K^{\top}) has three columns), with the right-hand side made up of the corresponding columns of ![equation](http://latex.codecogs.com/gif.latex?HP^{-}).

## Test case

With initial:

`q = [0; 0; 0; 1]`

`P = zeros(3)`

and with parameters

`Q = diag(0.1)`

`R = diag(0.1)`

`Ts = 0.05`

and measurements

`omega = [0; 0.4363; -0.0873]`

`y = [-0.3660; -0.5753; -0.7315; 0.5844; 0.2708; 0.765]`

and reference vectors

`r1 = [0.3554; 0.0734; -0.9318]`

`r2 = [0; 0; 1]`

we should expect to see a Predict step result in (or close to)

`q = [0; 0.0109; -0.0022; 1]`

`P = diag(0.005)`

and an Update step result in (or close to)

`q = [0.0195; -0.0167; 0.0022; 0.9997]`

`P = [0.0046, 0, -0.0001; 0, 0.0045, 0; -0.0001, 0, 0.005]`

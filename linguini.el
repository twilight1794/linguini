;; Definición del modo 
;;;###autoload
(define-derived-mode linguini-mode
  prog-mode "Linguini"
  "Toggle Paws mode.
Interactivamente sin argumento, este comando des/activa el modo."
  ;; Colorear sintaxis
  (setq font-lock-defaults '((linguini-font-lock-keywords))))
(setq linguini-font-lock-keywords
      (let* (
            ;; definir algunas categorías de keywords
             (x-keywords '("Si" "SiNo" "Mientras" "Programa" "AsignarA" "Fin" "Leer" "Escribir" "Continuar" "Regresar" "Salir"))
            (x-types '("Numero" "Cadena" "Logico"))
            (x-constants '("Verdadero" "Falso"))
            ;(x-events '("OREF" "ACTION" "WRITE" "WRITELN" "OBJECT" "ATTR" "LISTCONTENTS"))
            (x-functions '("absoluto" "truncar" "redondear" "sin" "cos" "tan" "arcsin" "arccos" "arctan"  "sinh" "cosh" "tanh" "asinh" "acosh" "atanh" "log" "ln" "exp" "factoial" "aleatorio" "mayuscula" "minuscula" "esalnum" "esnumero" "esletra" "esmayuscula" "esminuscula" "textoanum" "numatexto" "ascii"))

            ;; generar las cadenas regex para cada categoría de keywords
            (x-keywords-regexp (regexp-opt x-keywords 'words))
            (x-types-regexp (regexp-opt x-types 'words))
            (x-constants-regexp (regexp-opt x-constants 'words))
            ;(x-events-regexp (regexp-opt x-events 'words))
            (x-functions-regexp (regexp-opt x-functions 'words)))
)
        `(
          (,x-types-regexp . font-lock-type-face)
          (,x-constants-regexp . font-lock-constant-face)
          ;(,x-events-regexp . font-lock-builtin-face)
          ;(,x-functions-regexp . font-lock-function-name-face)
          (,x-keywords-regexp . font-lock-keyword-face)
          )))
(setq linguini-mode-syntax-table
      (let ((tabla (make-syntax-table)))
        (modify-syntax-entry ?\# "<" tabla)
        (modify-syntax-entry ?\n ">" tabla)
        tabla))

(defun linguini-mode-variables (&opcional syntax keywords-case-insensitive)
  (when syntax
    (set-syntax-table linguini-mode-syntax-table)))
(setq auto-mode-alist
      (append '(("\\.ling\\'" . linguini-mode)) auto-mode-alist))

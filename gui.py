"""
user graphical interface
"""

import os
import subprocess
import threading
import tkinter as tk
from tkinter import messagebox, ttk

from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg

from BL_cpp.ofp.plot_sw_with_noise import plot as plot_with_noise


class Application(tk.Frame):
    def __init__(self, master=None):
        super().__init__(master)
        self.master = master
        self.pack()
        self.create_widgets()
        self.__path_to_params_h = 'BL_cpp/source/params.h'

    def create_widgets(self):
        self.nw = tk.StringVar()
        self.nw.set('2.5638')
        self.no = tk.StringVar()
        self.no.set('3.5')

        self.create_frame_set_params_h()
        self.add_input_nw_no()
        self.build_button = tk.Button(self)
        self.build_button["text"] = "Build"
        self.build_button["command"] = self.build
        self.build_button.pack(side="left")

        self.run_static_button = tk.Button(self)
        self.run_static_button["text"] = "Run Static"
        self.run_static_button["command"] = self.run_static
        self.run_static_button.pack(side="left")

        self.add_noise_button = tk.Button(self)
        self.add_noise_button["text"] = "Add Noise"
        self.add_noise_button["command"] = self.add_noise
        self.add_noise_button.pack(side="left")

        self.plot_button = tk.Button(self)
        self.plot_button["text"] = "Plot"
        self.plot_button["command"] = self.plot
        self.plot_button.pack(side="left")

        self.output_label = tk.Label(self.master)
        self.output_label.pack(side="top")

        self.reverse_solver_button = tk.Button(self)
        self.reverse_solver_button["text"] = "Reverse solver"
        self.reverse_solver_button["command"] = self.reverse_solver
        self.reverse_solver_button.pack(side="right")

    def add_input_nw_no(self):
        self.input_nw_no_frame = tk.Frame(self)
        tk.Label(self.input_nw_no_frame, text='nw:'
                 ).grid(row=0, column=0, ipady=10, sticky='e')
        tk.Label(self.input_nw_no_frame, text='no:'
                 ).grid(row=1, column=0, ipady=10, sticky='e')

        tk.Entry(self.input_nw_no_frame, textvariable=self.nw).grid(row=0, column=1, ipady=10, sticky='w')
        tk.Entry(self.input_nw_no_frame, textvariable=self.no).grid(row=1, column=1, ipady=10, sticky='w')

        tk.Button(
            self.input_nw_no_frame,
            text='Enter',
            command=lambda: print(self.nw.get(), self.no.get())
        ).grid(row=2, column=0, columnspan=2, sticky='nsew')

        self.input_nw_no_frame.pack(side='bottom')

    def build(self):
        threading.Thread(target=self._build).start()

    @staticmethod
    def _build():
        subprocess.call(["cmake", ".."], cwd="BL_cpp/build")
        ret = subprocess.call(["make"], cwd="BL_cpp/build")
        if ret != 0:
            messagebox.showerror("Error", "Build failed")
        else:
            messagebox.showinfo(title="Complete!", message="Build Completed!")

    def run_static(self):
        threading.Thread(target=self._run_static).start()

    def _run_static(self):
        _nw = self.nw.get()
        _no = self.no.get()
        _params = _nw + ' ' + _no
        output = subprocess.check_output(["./BL_cpp", _nw, _no], cwd="BL_cpp/build")
        output_str = output.decode("utf-8")
        self.output_label.configure(text=output_str)
        # ret = os.system("./BL_cpp/build/BL_cpp")
        # if ret != 0:
        #     messagebox.showerror("Error", "Execution failed")

    def add_noise(self):
        threading.Thread(target=self._add_noise).start()

    def _add_noise(self):
        input_file = os.path.join("..", "ofp", "ofp_new")
        output_file = os.path.join("..", "ofp", "ofp_with_noise")
        subprocess.call(["./add_noise", input_file, output_file], cwd="BL_cpp/build")


    def plot(self):
        threading.Thread(target=self._plot).start()

    def _plot(self):
        fig = plot_with_noise("BL_cpp/ofp/ofp_with_noise")

        self.canvas = FigureCanvasTkAgg(fig, master=self.master)
        self.canvas.draw()
        self.canvas.get_tk_widget().pack()

    def reverse_solver(self):
        threading.Thread(target=self._reverse_solver).start()

    def _reverse_solver(self):
        input_file = os.path.join("..", "ofp", "ofp_with_noise")
        output = subprocess.check_output(["./reverse_noise", input_file], cwd="BL_cpp/build")
        output_str = output.decode("utf-8")
        new_output_str = output_str.split('\n')
        str_for_output = ''

        for i in range(10, 1, -1):
            str_for_output = str_for_output + new_output_str[-i] + '\n'

        self.output_label.configure(text=str_for_output)

    def create_frame_set_params_h(self):
        self.set_params_h_frame = tk.Frame(self).pack(side='bottom')
        txt_field = tk.Text(self.set_params_h_frame, width=100)

        buttons = tk.Frame(self.set_params_h_frame)

        button_open_file = tk.Button(
            buttons, 
            text='Открыть',
            command=lambda: self.click_button_open_file(
            text_field=txt_field)
            )
        
        button_save_file = tk.Button(buttons, text='Сохранить как',
                                     command=lambda: self.click_button_save_file(
                                         text_field=txt_field
                                     ))

        button_open_file.grid(row=0, column=0, sticky='ew', padx=5, pady=5)
        button_save_file.grid(row=0, column=1, sticky='ew', padx=5)

        buttons.pack()
        txt_field.pack()
        txt_field.bind('<Control-s>', lambda action: self.click_button_save_file(text_field=txt_field))
        txt_field.bind('<Control-o>', lambda action: self.click_button_open_file(text_field=txt_field))
        txt_field.focus()
        scrollbar = ttk.Scrollbar(self.set_params_h_frame, orient=tk.VERTICAL, command=txt_field.yview)
        txt_field.configure(yscroll=scrollbar.set)
        scrollbar.pack(side='right', fill='y')


    def click_button_open_file(self, text_field):
        with open(self.__path_to_params_h, 'r') as input_file:
            text = input_file.read()
            text_field.insert(tk.END, text)

    def click_button_save_file(self, text_field):
        with open(self.__path_to_params_h, 'w') as output_file:
            text = text_field.get("1.0", tk.END)
            output_file.write(text)


root = tk.Tk()
root.geometry("1000x1000")
app = Application(master=root)
app.mainloop()

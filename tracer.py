"""
Usage : ./source go-back-n {ip} {porta} {portb} | python3 tracer.py -[s v t p]
s : Same graph, tout les tracés sont sur la même figure
v : Donne un nom aux axes et écris la légende (déconseillé sans -s)
t : Enlève les ticks des axes, conseillé si le résultat est peu lisible sans -s
p : Rajoute des points indiquant chaque lecture de fenêtre
    - vert : aquittement en séquence
    - rouge : Timeout
    - bleu : Trois ack successif
    - Jaune : ECN
La source doit afficher la fenêtre de congestion au format :
    "!%d %d\n", id_flux, taille fenêtre (si avec -p tout les points seront vert) 
    ou 
    "!%d %d %d\n", id_flux, taille fenêtre, type évènement (0 : ack, 1 : timeout, 2 : 3 ack, 3 : ecn)

Les sorties n'étant pas précédé par ! ou celles sur stderr ne sont pas lu.
"""

import matplotlib.pyplot as plt
import time
import random
import sys
import getopt


class FluxEvol:
    same_graph = False
    verbose = False
    noticks = False
    withpoints = False
    list_style = {0: "red", 1: "green", 2: "blue", 3: "magenta"}
    point_style = ["g", "r", "b", "yellow"]

    min_time = None
    max_time = None
    min_cong = 0
    max_cong = 5

    nb_col = 1
    nb_row = 4
    l_flux = []

    def __init__(self, flux, firstcong, ev):
        self.id_flux = flux
        FluxEvol.l_flux.append(self)
        if len(FluxEvol.l_flux) == FluxEvol.nb_col * FluxEvol.nb_row:
            FluxEvol.nb_row += 1
            FluxEvol.nb_col *= 2

        self.congestions = [firstcong]
        if FluxEvol.min_time is None:
            FluxEvol.min_time = time.time()
            FluxEvol.max_time = 0
        else:
            FluxEvol.max_time = time.time() - FluxEvol.min_time
        self.time_measure = [FluxEvol.max_time]
        if flux in FluxEvol.list_style:
            self.style = FluxEvol.list_style[flux]
        else:
            self.style = "#%02x%02x%02x" % (
                random.randrange(256),
                random.randrange(256),
                random.randrange(256),
            )
        self.event_style = [FluxEvol.point_style[ev]]

        if firstcong > FluxEvol.max_cong:
            FluxEvol.max_cong = firstcong

    def append(self, cong_size, ev):
        if cong_size > FluxEvol.max_cong:
            FluxEvol.max_cong = cong_size

        self.congestions.append(cong_size)
        FluxEvol.max_time = time.time() - FluxEvol.min_time
        self.time_measure.append(FluxEvol.max_time)
        self.event_style.append(FluxEvol.point_style[ev])

    def plot(self):
        nb_measure = len(self.time_measure)
        ech_0 = nb_measure - min(25, nb_measure / 4)
        ech_1 = nb_measure + min(25, nb_measure / 4)
        # print(
        #    f"For flux {self.id_flux} {nb_measure} measure:",
        #    self.time_measure[ech_0:ech_1],
        #    self.congestions[ech_0:ech_1],
        #    sep="\n",
        # )
        if not FluxEvol.same_graph:
            plt.subplot(
                len(FluxEvol.l_flux) // FluxEvol.nb_col + 1,
                FluxEvol.nb_col,
                FluxEvol.l_flux.index(self) + 1,
            )
            plt.gca().set(
                xlim=(min(self.time_measure), max(self.time_measure)),
                ylim=(FluxEvol.min_cong, max(self.congestions) * 1.05),
            )
        else:
            plt.gca().set(
                xlim=(0, FluxEvol.max_time),
                ylim=(FluxEvol.min_cong, FluxEvol.max_cong),
            )
        if FluxEvol.verbose:
            plt.ylabel("Taille de la fenêtre de congestion en MSS")
            plt.xlabel("Temps depuis premier segment DATA en seconde")
        if FluxEvol.noticks:
            plt.gca().set_yticks([])
            plt.gca().set_xticks([])
        if FluxEvol.withpoints:
            plt.scatter(
                self.time_measure,
                self.congestions,
                c=self.event_style,
            )
        plt.plot(
            self.time_measure,
            self.congestions,
            self.style,
            label=f"Flux ({self.id_flux})",
        )
        if FluxEvol.verbose:
            plt.legend()


if __name__ == "__main__":
    opts, args = getopt.getopt(
        sys.argv[1:],
        "hsvtp",
        ["help", "samegraph", "verbose", "noticks", "points"],
    )
    for o, a in opts:
        if o in ("-h", "--help"):
            help()
            sys.exit()
        elif o in ("-s", "--samegraph"):
            FluxEvol.same_graph = True
        elif o in ("-v", "--verbose"):
            FluxEvol.verbose = True
        elif o in ("-t", "--noticks"):
            FluxEvol.noticks = True
        elif o in ("-p", "--points"):
            FluxEvol.withpoints = True
        else:
            print("WTD")
            sys.exit(2)
    list_flux = {}
    input_open = True
    while True:
        try:
            txt = input()
        except EOFError:
            input_open = False
            print("EOF")
        except KeyboardInterrupt:
            print("Interrupt")
            input_open = False
        if not input_open:
            break

        if len(txt) > 0 and txt[0] == "!":
            try:
                result = list(map(int, txt[1:].split()))
                if len(result) == 2:
                    id_flux, cong, ev = result + [0]
                elif len(result) == 3:
                    id_flux, cong, ev = result
                else:
                    print(f"Problem decoding too much data {txt}", file=sys.stderr)
                    continue
            except:
                print(f"Problem decoding {txt}", file=sys.stderr)
                continue
            if id_flux in list_flux:
                list_flux[id_flux].append(cong, ev)
            else:
                list_flux[id_flux] = FluxEvol(id_flux, cong, ev)

    for id_flux in list_flux:
        list_flux[id_flux].plot()
    plt.savefig("figure.png")
    plt.show()
